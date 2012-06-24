#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;
uniform vec4 mouse;
uniform sampler2D tex0;

void main(void)
{
    vec2 halfres = resolution.xy/2.0;
    vec2 cPos = gl_FragCoord.xy;

    cPos.x -= 0.5*halfres.x*sin(time/2.0)+0.3*halfres.x*cos(time)+halfres.x;
    cPos.y -= 0.4*halfres.y*sin(time/5.0)+0.3*halfres.y*cos(time)+halfres.y;
    float cLength = length(cPos);

    vec2 uv = gl_FragCoord.xy/resolution.xy+(cPos/cLength)*sin(cLength/30.0-time*10.0)/25.0;
    vec3 col = texture2D(tex0,uv).xyz*50.0/cLength;

    gl_FragColor = vec4(col,1.0);
}

