#ifndef GEW_H
#define GEQ_H

#define EQ_BANDS 30

void bind_geq();
void geq_set_sliders(int length, float x[], float y[]);

/* Linear gain of the 1/3rd octave EQ bands */
extern float geq_gains[];
/* Frequency of each band of the EQ */
extern float geq_freqs[];

#endif
