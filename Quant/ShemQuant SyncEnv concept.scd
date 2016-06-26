p = ProxySpace.push(s).makeTempoClock;
SynthDef(\envControl, { |bus, tempo| ReplaceOut.kr(bus, EnvGen.kr(\env.kr( Env.newClear().asArray ), timeScale:tempo.reciprocal, doneAction: 2)); }).add;
s.scope(3,0, rate:\control);

~a.play(vol:0.1);
~a.set(\amp, BusPlug.for(1));
~a[0] = {SinOsc.ar(60!2, mul:\amp.kr(0));}
(
~a[1] = Task {
	var quant = 4;
	p.clock.timeToNextBeat(quant).wait;
	loop {
		"time_flag4".postln;
		Synth(\envControl, [\bus: 1, \tempo: p.clock.tempo, \env: [Env([0,1,0], [0.05,3.85], \lin)]]);
		quant.wait;
	}
}
)
~a.stop

~b.play(vol:0.2);
~b.set(\amp, BusPlug.for(2));
~b[0] = {SinOsc.ar(90!2, mul:\amp.kr(0))}
(
~b[1] = Task {
	var env = Env([0,1,0], [0.05,0.25], \lin);
	var quant = 2;
	p.clock.timeToNextBeat(quant).wait;
	loop {
		"time_flag2".postln;
		Synth(\envControl, [\bus: 2, \tempo: p.clock.tempo, \env: [env]]);
		quant.wait;
	}
}
)
~b.stop

p.clock.tempo_(120/60);


