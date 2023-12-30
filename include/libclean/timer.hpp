#ifndef LIBCLEAN_TIMER_HPP
#define LIBCLEAN_TIMER_HPP

class Timer {
    public:
        static int time;
        Timer();
        void addTime();
        int getTime();
};

#endif
