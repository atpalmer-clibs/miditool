#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BPM_TO_MICROS(bpm) ((uint32_t)((60.0 * 1000000.0)/(double)bpm))

#define CHANNEL(channel) ((uint8_t)(channel - 1))

enum pitch {
    PITCH_G3 = 0x37,
    PITCH_C4 = 0x3C,
    PITCH_Db4 = 0x3D,
    PITCH_D4 = 0x3E,
    PITCH_Eb4 = 0x3F,
    PITCH_E4 = 0x40,
    PITCH_F4 = 0x41,
    PITCH_Gb4 = 0x42,
    PITCH_G4 = 0x43,
    PITCH_Ab4 = 0x44,
    PITCH_A4 = 0x45,
    PITCH_Bb4 = 0x46,
    PITCH_B4 = 0x47,
    PITCH_C5 = 0x48,
};

enum key_tonality {
    KEY_TONALITY_MAJOR = 0,
    KEY_TONALITY_MINOR = 1,
};

#define KEY_SHARPS(count) ((uint8_t)count)
#define KEY_FLATS(count) ((uint8_t)(-1 * (count)))

enum timesig_denom {
    TIMESIG_DENOM_1 = 0,
    TIMESIG_DENOM_2 = 1,
    TIMESIG_DENOM_4 = 2,
    TIMESIG_DENOM_8 = 3,
    TIMESIG_DENOM_16 = 4,
    TIMESIG_DENOM_32 = 5,
};

enum velocity {
    VELOCITY_MAX = 0x7F,
};

enum midi_format {
    FORMAT_SINGLE_TRACK = 0,
    FORMAT_MULTI_TRACK = 1,
    FORMAT_MULTI_SONG = 2,  /* i.e. series of type 0 files */
};

#define DIVISION_TICKS_PER_BEAT(ticks) (ticks)
#define DIVISION_SMPTE(value) (-1 * (value))

#endif
