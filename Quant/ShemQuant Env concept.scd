p = ProxySpace.push(s)
s.scope(3,0, rate:\control);

SynthDef(\envControl, { |bus| Out.kr(bus, EnvGen.kr(\env.kr( Env.newClear().asArray ), doneAction: 2)); }).add;
~sig.play(vol:0.5).quant_(4);

Bus.new(\control, 0, 1, s).set(1);

(
~sig[0] = { SinOsc.ar(60 + \freq.kr(0).range(0,60)!2, mul:\amp.kr(0.1)) * \masterVolume.kr(1)};

~sig.set(\masterVolume, BusPlug.for(0));
~sig.set(\amp, BusPlug.for(1));
~sig.set(\freq, BusPlug.for(2));
)

~sig[1] = Pbind(\instrument, \envControl, \bus, 1, \dur, 2, \env, [Env([0,1,0], [0.05,1.85], \lin)]);
~sig[2] = Pbind(\instrument, \envControl, \bus, 2, \dur, 0.5, \env, [Env([0.01,0.9,0.01], [0.05,0.15], \exp)]);

(
var loop = 0;
var pattern = Pseq([0,0,0,1], inf);
var envs = Pswitch([
	[Env([0.01,0.9,0.01], [0.05,0.45], \exp)],
	[Env([0.01,0.9,0.01], [0.45,1.55], \exp)]
], pattern);

~sig[2] = Pbind(
	\instrument, \envControl,
	\bus, 2,
	\env, envs,
	\dur, Pfunc({
		var currentNum = envs.which.list[loop % envs.which.list.size];
		var currentEnv = envs.list[currentNum];

		loop = loop + 1;
		("env time" + currentEnv[0].totalDuration).postln;

		currentEnv[0].totalDuration;
	})

);
)


~sig[2] = nil

Synth(\envControl, [\bus, 1, \env, Env([1,0], 8, \sin)]);

~sig.free



