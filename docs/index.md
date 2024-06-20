# Csound Plugins documentation


This is a collection of plugins for [csound](https://csound.com/)



## Ableton Link opcodes

The purpose of Ableton Link is to synchronize musical time, beat, and phase between musical applications performing in real time from separate programs, processes, and network addresses. This is useful, e.g., for laptop orchestras.

Ableton Live is not required to use the Ableton Link protocol, as it is a peer-to-peer protocol. There is one Link session on the local area network that maintains a global time, tempo, and beat. Any peer may set the tempo, and thereafter all peers in the session share that tempo. A process may have any number of peers (i.e., any number of Link objects). Each peer may also define its own "quantum" i.e. some multiple of the beat, e.g. a quantum of 4 might imply 1 beat every measure of 4/4 time. The phase of the time is defined w.r.t the quantum, e.g. phase 0.5 of a quantum of 4 would be the second beat of the measure. Peers may read and write timelines with local time, beat, and phase, counting from when the peer is enabled, but the tempo and beat on all timelines for all peers in the session will coincide.

The first peer in a session determines the initial tempo. After that, the tempo is changed only, and whenever, any peer explicity calls the set tempo functon (link_tempo_set, in Csound).

The Link tempo is independent of the Csound score tempo. Performances that need to synchronize the score tempo with the Link tempo may use the tempo opcode to set the score tempo from the Link tempo; or conversely, set the Link tempo from the score tempo using the tempoval opcode.

Please note, the phase and beat obtained or set by these opcodes is only as precise as allowed by the duration of Csound's kperiod, the audio driver used by Csound, network latency and stability, and the system's most precise clock.

* [link_create](opcodes/link_create.md): Creates an Ableton Link peer object.
* [link_enable](opcodes/link_enable.md): Enable/disable synchronization with the network Ableton Link session tempo and beat.
* [link_is_enabled](opcodes/link_is_enabled.md): Returns whether or not this Ableton Link peer has joined the network session. 
* [link_tempo_set](opcodes/link_tempo_set.md): Sets the tempo for the network's Ableton Link session.
* [link_tempo_get](opcodes/link_tempo_get.md): Returns the tempo of the network's Ableton Link session.
* [link_beat_get](opcodes/link_beat_get.md): Returns the beat, phase, and current time of Ableton Link for this session for a given quantum.
* [link_metro](opcodes/link_metro.md): Returns a trigger that is 1 on the beat and 0 otherwise along with the beat, phase, and current time of Ableton Link for this session for a given quantum.
* [link_beat_request](opcodes/link_beat_request.md): Requests a beat with a specific number at a specific time at a given quantum.
* [link_beat_force](opcodes/link_beat_force.md): Forces the network Ableton Link session to adopt a beat with a specific number at a specific time at a given quantum.
* [link_peers](opcodes/link_peers.md): Returns the number of peers currently joined in the network Ableton Link session.


## Bela

A sound and sensor technology

* [trill](opcodes/trill.md): Create a hashtable 


## CUDA

A parallel computing platform and application programming interface that allows software to use certain types of graphics processing units (GPUs) for accelerated general-purpose processing,

* [cudanal](opcodes/cudanal.md): Generate an fsig from a mono audio source ain, using phase vocoder overlap-add analysis and GPU hardware.
* [cudasynth](opcodes/cudasynth.md): Synthesis by additive synthesis and inverse FFT. 


## Faust opcodes

The Faust opcodes allows access from within Csound to the Faust language. Faust (Functional Audio Stream) is a functional programming language for sound synthesis and audio processing: https://faust.grame.fr/.

* [faustaudio](opcodes/faustaudio.md): Instantiates and runs a compiled Faust program.
* [faustcompile](opcodes/faustcompile.md): Produce a instantiable DSP process from a Faust program.
* [faustdsp](opcodes/faustdsp.md): Instantiates a Faust program.
* [faustctl](opcodes/faustctl.md): Adjusts a given control in a Faust DSP instance.
* [faustgen](opcodes/faustgen.md): Compiles, Instantiates and runs a compiled Faust program.
* [faustplay](opcodes/faustplay.md): Runs an instantiated Faust program.


## jsfx

jsfx support for csound

* [jsfx](opcodes/jsfx.md): Instantiates and runs a jsfx script 
* [jsfx_new](opcodes/jsfx_new.md): Instantiates a jsfx script 
* [jsfx_play](opcodes/jsfx_play.md): Processes audio through a jsfx script 
* [jsfx_getslider](opcodes/jsfx_getslider.md): Gets a slider value of a jsfx instance 
* [jsfx_setslider](opcodes/jsfx_setslider.md): Sets the slider values of a jsfx script 
* [tubeharmonics](opcodes/tubeharmonics.md): A distortion with control for odd/even harmonics 


## chua

opcodes using libsndfile

* [chuap](opcodes/chuap.md): Simulates Chua's oscillator, an LRC oscillator with an active resistor, proved capable of bifurcation and chaotic attractors.


## Fluid opcodes

The fluid family of opcodes wraps Peter Hannape's SoundFont 2 player

* [fluidAllOut](opcodes/fluidAllOut.md): Receiving audio from all FluidSynth engines.
* [fluidCCi](opcodes/fluidCCi.md): Sending a controller message at i-time to a FluidSynth engine's MIDI channel.
* [fluidCCk](opcodes/fluidCCk.md): Sending a controller message at k-rate to a FluidSynth engine's MIDI channel.
* [fluidControl](opcodes/fluidControl.md): Playing and controlling loaded Soundfonts (using 'raw' MIDI messages).
* [fluidEngine](opcodes/fluidEngine.md): Instantiating a FluidSynth engine.
* [fluidLoad](opcodes/fluidLoad.md): Loading SoundFonts.
* [fluidNote](opcodes/fluidNote.md): Playing a note on a FluidSynth engine's MIDI channel.
* [fluidOut](fluidOut.md): Receiving audio from a single FluidSynth engine. 
* [fluidProgramSelect](opcodes/fluidProgramSelect.md): Assigning presets from a SoundFont to a FluidSynth engine's MIDI channel. 
* [fluidSetInterpMethod](opcodes/fluidSetInterpMethod.md): setting interpolation method for a channel in a FluidSynth engine.

  ## Fluid opcodes
  
* [findarray](opcodes/findarray.md): Find an element in an array 
* [frac2int](opcodes/frac2int.md): Convert the fractional part of a number into an integer 
* [ftfill](opcodes/ftfill.md): create a table and fill it with values (like fillarray but for f-tables) 
* [ftfind](opcodes/ftfind.md): Find an element in a table 
* [ftnew](opcodes/ftnew.md): creates a new table of a given size 
* [ftsetparams](opcodes/ftsetparams.md): Set metadata parameters of a table, as if it was loaded via GEN1 
* [initerror](opcodes/initerror.md): Throws an error message at init 
* [interp1d](opcodes/interp1d.md): Interpolate between elements of an array/table 
* [lfnoise](opcodes/lfnoise.md): low frequency, band-limited noise 
* [linenv](opcodes/linenv.md): A triggerable linear envelope with sustain segment 
* [loadnpy](opcodes/loadnpy.md): Load an array (of any number of dimensions) saved as a .npy file 
* [memview](opcodes/memview.md): Create a view into a table or another array 
* [mtro](opcodes/mtro.md): an accurate version of metro 
* [nametoinstrnum](opcodes/nametoinstrnum.md): Returns the number of a named instrument 
* [panstereo](opcodes/panstereo.md): Stereo signal balancer 
* [perlin3](opcodes/perlin3.md): gradient noise sound generator 
* [pread](opcodes/pread.md): Read pfield values from any active instrument instance 
* [pwrite](opcodes/pwrite.md): Modify pfield values of an active instrument instance 
* [pwriten](opcodes/pwriten.md): Modify pfield values of an active instrument instance created via nstance 
* [presetinterp](opcodes/presetinterp.md): Interpolates between presets placed in a 2D plane 
* [ramptrig](opcodes/ramptrig.md): A triggerable ramp between 0 and 1 
* [ref](opcodes/ref.md): Get a reference to a variable 
* [refvalid](opcodes/refvalid.md): Queries if a reference is valid 
* [schmitt](opcodes/schmitt.md): A schmitt trigger (a comparator with hysteresis). 
* [setslice](opcodes/setslice.md): Set a slice of an array to a given value 
* [sigmdrive](opcodes/sigmdrive.md): Analog "soft clipping" distortion by applying non-linear transfer functions. 
* [standardchaos](opcodes/standardchaos.md): Standard map chaotic generator 
* [throwerror](opcodes/throwerror.md): Throws an error message at performance or init 
* [uniqinstance](opcodes/uniqinstance.md): Return an fractional instrument number which is not in use 
* [vowelsdb](opcodes/vowelsdb.md): A database of vowel sounds 
* [weightedsum](opcodes/weightedsum.md): Weighted sum of multiple 1D arrays, or the rows of a 2D array 
* [zeroarray](opcodes/zeroarray.md): Zero all elements in an array 
* [zerocrossing](opcodes/zerocrossing.md): Outputs a frequency based on the number of zero-crossings per second. 



