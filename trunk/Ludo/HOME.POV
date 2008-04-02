#version 3.02
#include "colors.inc"

global_settings {
	assumed_gamma 1.33
}

camera {
	location <-2,3.5,-4>
//	location <0,0,-4>
	look_at <0,1,0>
}

light_source {
	<-.7,1,-1>*2
	color White
	area_light <1,0,-1>*.02,<0,1,0>*.02,3,3
	adaptive on
	jitter
}

light_source {
	<-1,1,-1>*2
	color White*.5
	shadowless
}

prism {
  conic_sweep
  linear_spline
  0, 1, 5,
  <4,4>,<-4,4>,<-4,-4>,<4,-4>,<4,4>
  rotate <180, 0, 0>
  translate <0, 1, 0>
  scale <1/4, 1,1/4>*1.2
	pigment {color Red}
	translate <0,2,0>
}

box {-1,1
	pigment {color Green}
	translate <0,1,0>
}

plane {y,0
	pigment {color White*.7}
	finish {crand .07 reflection .3}
}

#declare Radius=.3
#declare Peg=
merge {
	cylinder {<0,0,-Radius>,<0,0,Radius>,1}
	torus {1,Radius rotate <90,0,0>}
	finish {specular 1}
}

#declare Pillar=
merge {
	cylinder {<0,0,0>,<0,.7,0>,.2}
	sphere {0,.2 translate <0,.7,0>}
	cylinder {<0,0,0>,<0,2,0>,.1}
	cone {
		<0,2,0>,.15,<0,2.3,0>,0
		pigment {color Blue}
	}
	pigment {color Green}
}

object {
	Pillar
	translate <-1.5,0,-1.5>
}
object {
	Pillar
	translate <1.5,0,-1.5>
}
object {
	Pillar
	translate <-1.5,0,1.5>
}
object {
	Pillar
	translate <1.5,0,1.5>
}

box {
	<-2,0,-2>,<2,.3,2>
	pigment {color Green}
}

text {
	ttf "C:\WINDOWS\FONTS\COMIC.TTF",
	"HOME",.1,0
	scale .7
	translate <-1,1,-1.2>
	pigment {color Yellow}
}
