/*
 * TiMidity -- Experimental MIDI to WAVE converter
 * Copyright (C) 1995 Tuukka Toivonen <toivonen@clinet.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * resample.h
 */

#ifndef TIMIDITY_RESAMPLE_H
#define TIMIDITY_RESAMPLE_H

#define resample_voice TIMI_NAMESPACE(resample_voice)
#define pre_resample TIMI_NAMESPACE(pre_resample)

extern sample_t *resample_voice(MidSong *song, int v, sint32 *countptr);
extern void pre_resample(MidSong *song, MidSample *sp);

#endif /* TIMIDITY_RESAMPLE_H */
