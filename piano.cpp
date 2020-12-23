#ifdef __arm__
#define PWM_PATH "/sys/class/pwm/pwm0/period"
#define PWM_ENABLE "/sys/class/pwm/pwm0/enable"
#elif __powerpc__
#define PWM_PATH "/proc/stw_beep/beep"
#define PWM_ENABLE PWM_PATH
#else
#error "Unsupported architecture"
#endif

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <termios.h>

char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}

void stop()
{
    std::ofstream f;
    f.open(PWM_ENABLE);
    f << '0';
    f.close();
}

void play(unsigned freq)
{
    std::ofstream f;
    f.open(PWM_PATH);
#ifdef __arm__
    f << (unsigned)1e9 / freq;
    f.close();
    f.open(PWM_ENABLE);
    f << '1';
#else
    f << freq;
#endif
    f.close();
    usleep(250000);
    stop();
}

int main()
{
    std::cout << "Piano activated - press ESC to exit\n";
    char ch;
    while (ch != '\e')
    {
        ch = getch();
        switch (ch)
        {
        case 'q':
            // C0
            play(262);
            break;
        case 'w':
            // D0
            play(293);
            break;
        case 'e':
            // E0
            play(329);
            break;
        case 'r':
            // F0
            play(349);
            break;
        case 't':
            // G0
            play(392);
            break;
        case 'y':
            // A0
            play(440);
            break;
        case 'u':
            // B0
            play(493);
            break;
        case 'i':
            // C1
            play(523);
            break;
        case 'o':
            // D1
            play(587);
            break;
        case 'p':
            // E1
            play(659);
            break;
        case '2':
            // C#0
            play(277);
            break;
        case '3':
            // D#0
            play(311);
            break;
        case '5':
            // F#0
            play(370);
            break;
        case '6':
            // G#0
            play(415);
            break;
        case '7':
            // A#0
            play(466);
            break;
        case '9':
            // C#1
            play(554);
            break;
        case '0':
            // D#1
            play(622);
            break;
        default:
            stop();
        }
        usleep(10000);
    }
    stop();
    std::cout << std::flush;
}
