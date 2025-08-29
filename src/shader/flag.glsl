void mainImage(out vec4 O, vec2 U)
{
    float r = iResolution.y / 3.;

    // Colors
    vec4 blue = vec4(0., 169., 206., 255.) / 255.;   // Aquamarine
    vec4 yellow = vec4(253., 218., 37., 255.) / 255.; // Gold
    vec4 black = vec4(0., 0., 0., 1.);                // Black

    // Base color: top and bottom stripes (light blue)
    O = blue;

    // Middle stripe (yellow)
    if (U.y > r && U.y <= 2. * r)
    O = yellow;

    // Black triangle on left, pointing right
    float centerY = iResolution.y / 2.0;
    float triangleHeight = iResolution.y;
    float triangleWidth = triangleHeight * 0.57735027; // tan(30°) for equilateral triangle

    // Convert to triangle-relative space
    float dy = abs(U.y - centerY);
    float maxX = (centerY - dy) / tan(radians(60.)); // x boundary for triangle

    if (U.x <= maxX)
    O = black;
}
