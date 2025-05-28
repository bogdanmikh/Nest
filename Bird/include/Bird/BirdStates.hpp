#pragma once

namespace Bird {

#define BIT(x) (1 << x)

#define BIRD_STATE_CULL_FACE BIT(1)
#define BIRD_STATE_DEPTH_TEST BIT(2)
#define BIRD_STATE_DEPTH_CLAMP BIT(3)

#define BIRD_STATE_NO_AA BIT(8)
#define BIRD_STATE_MSAA_2X BIT(9)
#define BIRD_STATE_MSAA_4X BIT(10)
#define BIRD_STATE_MSAA_8X BIT(11)
#define BIRD_STATE_ALPHA_TO_COVERAGE BIT(12) // Alpha-to-Coverage (для прозрачности)
#define BIRD_STATE_SAMPLE_SHADING BIT(13)    // Sample Shading (высокое качество)

} // namespace Bird