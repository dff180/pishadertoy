#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;
uniform vec4 mouse;

void main(void)
{
    //the centre point for each blob
    vec2 move1;
    move1.x = cos(time)*0.4;
    move1.y = sin(time*1.5)*0.4;
    vec2 move2;
    move2.x = cos(time*2.0)*0.4;
    move2.y = sin(time*3.0)*0.4;
    
    //screen coordinates
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
  
    //radius for each blob
    float r1 =(dot(p-move1,p-move1))*8.0;
    float r2 =(dot(p+move2,p+move2))*16.0;

    //sum the meatballs
    float metaball =(1.0/r1+1.0/r2);
    //alter the cut-off power
    float col = pow(metaball,8.0);

    //set the output color
    gl_FragColor = vec4(col,col,col,1.0);
}

