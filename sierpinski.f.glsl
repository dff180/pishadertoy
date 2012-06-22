
#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;
uniform float time;
uniform sampler2D tex0;

// Set color at the current fragment, with given coords
// and whether it should be "hole" or not.
void setColor(vec2 coord, bool isHole) {
	if (isHole)
		gl_FragColor = vec4(texture2D(tex0, coord).xyz, 1.0);
	else
		gl_FragColor = vec4(coord.x, 0.5, coord.y, 1.0);
}

// Sierpinski carpet - with anti-holes!
// Maybe call it "Sierpinski tablecloth". If it doesn't already have a name.
void main(void)
{
	ivec2 sectors;
	vec2 coordOrig = gl_FragCoord.xy / resolution.xy;
	const int lim = 5;
	// Toggle between "carpet" and "tablecloth" every 3 seconds.
	bool doInverseHoles = (mod(time, 6.0) < 3.0);
	
	/* If you want it to spin, just to prove that it is redrawing
	the carpet every frame: */
	vec2 center = vec2(0.5, 0.5);
	mat2 rotation = mat2(
        vec2( cos(time), sin(time)),
        vec2(-sin(time), cos(time))
    );
    vec2 coordRot = rotation * (coordOrig - center) + center;
	// rotation can put us out of bounds
	if (coordRot.x < 0.0 || coordRot.x > 1.0 ||
		coordRot.y < 0.0 || coordRot.y > 1.0) {
		setColor(coordOrig, true);
		return;
	}

	vec2 coordIter = coordRot;
	bool isHole = false;
	
	for (int i=0; i < lim; i++) {
		sectors = ivec2(floor(coordIter.xy * 3.0));
		if (sectors.x == 1 && sectors.y == 1) {
			if (doInverseHoles) {
				isHole = !isHole;
			} else {
				setColor(coordOrig, true);
				return;
			}
		}

		if (i + 1 < lim) {
			// map current sector to whole carpet
			coordIter.xy = coordIter.xy * 3.0 - vec2(sectors.xy);
		}
	}
	
	setColor(isHole ? coordOrig : coordRot, isHole);
}

