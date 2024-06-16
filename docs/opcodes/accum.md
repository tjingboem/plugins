# link_create

## Abstract

link_create — Creates a peer in an Ableton Link network session.


## Description

Plugin opcode in ableton_link_opcodes. This opcode is part of the plugin repository and has to be installed separately. The plugin repository can be found here: https://github.com/csound/plugins

Creates a peer in an Ableton Link network session. The first peer in a session determines the initial tempo of the session. The value returned must be passed as the first parameter to all subsequent Ableton Link opcode calls for this peer.

## Syntax


```csound
i_peer link_create [i_bpm]

```
    
## Arguments


## Output


## Execution Time

* i_bpm -- Initial tempo of the session, in beats per minute. Has no effect unless this is the first peer in the session. The default value is 60.

* i_peer -- The Ableton Link peer object returned for use with the other Ableton Link opcodes.

## Examples


```csound

<CsoundSynthesizer>
<CsLicense>
Run the Ableton Link "LinkHut" example application, or some other 
Ableton Link peer, while you run this CSD to see what happens.
</CsLicense>
<CsOptions>
-m0 -d -odac 
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 10
nchnls = 2

alwayson "LinkMonitor"

gi_peer link_create 72
gk_beat init 0

instr Beep
asignal oscils 20000, p4, 0
outs asignal, asignal
endin

instr TempoChange
link_tempo_set gi_peer, 80
endin

instr LinkEnable
i_enabled = p4
link_enable gi_peer, i_enabled
endin

instr LinkMonitor
i_kperiod_seconds = ksmps / sr
printf_i "kperiod: %9.6f seconds.\n", 1, i_kperiod_seconds
printf_i "gi_peer: %g\n", 1, gi_peer
link_enable gi_peer, 1
k_trigger, gk_beat, k_phase, k_time link_metro gi_peer, 1
k_peers link_peers gi_peer
k_tempo link_tempo_get gi_peer
k_enabled link_is_enabled gi_peer
k_hz = 1000
if floor(gk_beat % 4) == 0 then
k_hz = 3000
else
k_hz = 2000
endif
schedkwhen k_trigger, 0, 1, "Beep", 0, 0.01, k_hz
printf "LinkMonitor: gi_peer: %g k_enabled: %9.4f k_trigger: %9.4f beat: %9.4f k_phase: %9.4f time: %9.4f tempo: %9.4f peers: %3d\n", k_trigger, gi_peer, k_enabled, k_trigger, gk_beat, k_phase, k_time, k_tempo, k_peers
endin
</CsInstruments>
<CsScore>
f 0 360
i "TempoChange" 10 80
i "LinkEnable"  20 1 0
i "LinkEnable"  30 1 1
</CsScore>
</CsoundSynthesizer>

```

```csound


<CsoundSynthesizer>
<CsOptions>
--nosound
</CsOptions>

<CsInstruments>

instr 1
  kx linseg 0, p3, 1
  printf "kx=%f \n", accum(changed(kx)), kx
  ; the same without accum would only print the first time,
  ; since changed would return always 1 but printf expects an ever
  ; increasing trigger
endin

</CsInstruments>
<CsScore>

i1 0 0.1

</CsScore>
</CsoundSynthesizer>


```


## See also

* [metro](http://www.csound.com/docs/manual/metro.html)
* [changed](http://www.csound.com/docs/manual/changed.html)
* [trighold](http://www.csound.com/docs/manual/trighold.html)
* [printf](http://www.csound.com/docs/manual/printf.html)

## Credits

Michael Gogins
