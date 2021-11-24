
inline __attribute__((always_inline)) int reload(char *addr) {
    volatile unsigned long time;
    asm __volatile__(
        "mfence \n"            // previous loads and stores are visible globally (memory fence)
        "lfence \n"            // previous loads are visible globally (load fence)
        "rdtscp \n"            // read timestamp counter - time is in the register eax
                               // rdtscp waits for previous instruction to be executed (rdtsc do not)
        "movl %%eax, %%esi \n" // move the timestamp from eax to esi
        "movl (%1), %%eax \n"  // movl move long from to input parameter to eax regixter
        "lfence \n"
        "rdtscp \n"
        "subl %%esi, %%eax \n"     // subtraction of timestamp one from timestamp two
        : "=a"(time)               // maps esi to local variable
        : "b"(addr)                // input variable
        : "%esi", "%edx", "%ecx"); // telling gcc what registers will be modified in the asm code
    return time;
}


inline __attribute__((always_inline)) void flush(char *addr) {
    asm __volatile__("mfence\n"
                     "clflush (%0)"         // flush cache line (clears address)
                     : /* no outputs */
                     : "r" (addr) 
                     : /* no clobber-list */ );
}
