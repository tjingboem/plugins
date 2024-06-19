# Csound Plugins documentation


This is a collection of plugins for [csound](https://csound.com/)



## Ableton Link opcodes

A technology that synchronizes musical beat, tempo, and phase across multiple applications running on one or more devices.

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


## beosc

Band-enhanced oscillators implementing the sine+noise synthesis model

* [beosc](opcodes/beosc.md): Band-Enhanced Oscillator 
* [beadsynt](opcodes/beadsynt.md): Band-Enhanced Oscillator-Bank 
* [getrowlin](opcodes/getrowlin.md): opy a row from a 2D array or table, with interpolation between rows 
* [tabrowlin](opcodes/tabrowlin.md): Copy a row from an f-table to another, interpolating between rows 


## jsfx

jsfx support for csound

* [jsfx](opcodes/jsfx.md): Instantiates and runs a jsfx script 
* [jsfx_new](opcodes/jsfx_new.md): Instantiates a jsfx script 
* [jsfx_play](opcodes/jsfx_play.md): Processes audio through a jsfx script 
* [jsfx_getslider](opcodes/jsfx_getslider.md): Gets a slider value of a jsfx instance 
* [jsfx_setslider](opcodes/jsfx_setslider.md): Sets the slider values of a jsfx script 
* [tubeharmonics](opcodes/tubeharmonics.md): A distortion with control for odd/even harmonics 


## sndmeta

opcodes using libsndfile

* [filereadmeta](opcodes/filereadmeta.md): Read metadata from a soundfile 


## else

Miscellaneous plugins

* [accum](opcodes/accum.md): Simple accumulator of scalar values 
* [atstop](opcodes/atstop.md): Schedule an instrument at the end of the current instrument 
* [bisect](opcodes/bisect.md): Returns the fractional index of a value within a sorted array / tab 
* [crackle](opcodes/crackle.md): generates noise based on a chaotic equation 
* [cuetrig](opcodes/cuetrig.md): Generate a trigger at given time values 
* [deref](opcodes/deref.md): Dereference a previously created reference to a variable 
* [detectsilence](opcodes/detectsilence.md): Detect when input falls below an amplitude threshold 
* [diode_ringmod](opcodes/diode_ringmod.md): A ring modulator with optional non-linearities 
* [extendarray](opcodes/extendarray.md): Extend one array with the contents of a second array, in place 
* [fileexists](opcodes/fileexists.md): Returns 1 if a file exists and can be read 
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



