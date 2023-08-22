#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texcoord;

uniform sampler2D _MainTex;
uniform sampler2D _MainTex2;
uniform float _mixValue;

void main()
{
    // FragColor = vec4( texture(_MainTex,texcoord).rgb * ourColor,1.0);


    //  FragColor = vec4( texture(_MainTex,texcoord *2.0).rgb * ourColor,1.0);

    vec4 color1 = texture(_MainTex,texcoord ); //笑脸
    vec4 color2 = texture(_MainTex2,texcoord);//方块
    FragColor = mix(color1,color2,_mixValue);

    // FragColor = color1;

    //朝向另外一个方向
    //  FragColor = vec4( texture(_MainTex,vec2(1.0-texcoord.x,texcoord.y)).rgb * ourColor,1.0);
    //vec4(ourColor, 1.0f);
}