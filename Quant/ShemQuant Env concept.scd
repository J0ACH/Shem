p = ProxySpace.push(s).makeTempoClock;
p.clock.tempo_(60/60);

(
//var nodeGroup = Group.new();
//("groupID" + nodeGroup.nodeID).postln;

//~test.synth.postln;
//.synthDef.nameDef(\ahoj);//.postln;
~test = NodeProxy.audio(s, 2);
~test.play(vol: 0.9, fadeTime: 1);
~test.group_(nodeGroup);

~test[0] = {
	var osc = SinOsc.ar(( \freq.kr(90) + \fplus.kr(0) ), mul: 1, add: 0);
	var noise = BrownNoise.ar(\noise.kr(0), (1 - \noise.kr(0)));
	var sig = osc * noise;
	Pan2.ar( sig * \amp.kr(1), \pan.kr(0) );
};

)

(
var busIndex = 10;
var bus = Bus.new(\control, busIndex, 1, s);
var cNode = NodeProxy.for(bus);
var pattern = Pseq([0,0,1,0,2], 2);
var envs = Pswitch([
	Env([0,1,0],[0.05,0.45]),
	Env([0,1,0],[0.75,0.25]),
	Env([0,1,0],[0.25,0.75])
], pattern );

//cNode.group_(nodeGroup);
//cNode.group_(Group.basicNew(s,1005));

~test[10] = Task({
	var cntLoops = envs.which.list.size * envs.which.repeats;
	cntLoops.do({|noLoop|
		var currentNum = envs.which.list[noLoop % envs.which.list.size];
		var currentEnv = envs.list[currentNum];

		~test.set(\amp, cNode.source_({
			EnvGen.kr(
				currentEnv,
				timeScale: currentEnvironment[\tempo].clock.tempo.reciprocal,
				doneAction:2
			)
		}));
		//noLoop.postln;
		//("currentNum" + currentNum).postcs;
		//currentEnv.postcs;

		currentEnv.totalDuration.wait;
	})
});
)











