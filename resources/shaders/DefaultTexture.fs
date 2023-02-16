#version 100

precision mediump float;

varying   vec2      v_v2TexCoord;
uniform   sampler2D u_2dTexture;

void main()
{
  gl_FragColor = texture2D(u_2dTexture, v_v2TexCoord);
}

