//progmix.c
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
    const char melody_notes[] = "IQNNNN!!]]!Q!IWJWQNN??!!W]WQNNN?";
    const double bass_notes[4] = { 3.0, 3.0, 4.75, 2.0 };
    double a, d, m, b, drum, melody, bass, sample;
    uint32_t t, w, v, p;
    for (t = 0; ; t++)
    {
        w = t >> 9;
        v = w >> 7;
        p = (w >> 5) & 0x03;
        a = 1.0 - fmod(t / 2048.0, 1.0);
        b = bass_notes[p] * t / 4.0;
        d = a * fmod((14 * t * t) ^ t, 2048.0);
        m = (melody_notes[((w >> 1) & 0x0F) | ((p / 3) << 4)] / 33.0 * t) - t;
        bass = fmod(b * 0.98, 80.0) + fmod(b, 80.0);
        drum = (((uint32_t)(fmod(5.0 * t, 2048.0) * a) & 128) * ((0x53232323 >> (w / 4)) & 1))
            + (((uint32_t)d & 127) * ((0xA444C444 >> (w / 4)) & 1)) + ((uint32_t)(d * w) & 1);
        melody = (fmod(m, 32.0) + fmod(m * 1.99, 32.0) + fmod(m * 0.49, 32.0) + fmod(m * 0.97, 32.0) - 64.0) * (4.0 - a - a - a);
        sample = bass + (a * ((v ? drum : 0.0) + (v > 1 ? melody : 0.0)));
        sample = (sample >= 0.0) ? sample : 0.0;
        putchar(((uint32_t)(sample * sample) >> 14) ? 127 : (int)sample);
    }
    return 0;
}
