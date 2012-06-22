uniform vec2 resolution;

void main(void) {
  gl_FragColor[0] = gl_FragCoord.x / resolution.x;
  gl_FragColor[1] = gl_FragCoord.y / resolution.y;
  gl_FragColor[2] = 0.5;
  gl_FragColor[3] = 1.0;
}
