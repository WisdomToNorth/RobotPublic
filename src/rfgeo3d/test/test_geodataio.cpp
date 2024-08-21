
#include <gp_Pnt.hxx>
#include <gtest/gtest.h>

#include "rfgeo3d/geodataio.h"

namespace rfgeo
{

TEST(geo3d_utils, dataio)
{
    // gp_Pnt
    RfJsonArray ptJson = {1.0, 2.0, 3.0};
    gp_Pnt pt = GeoDataIO::jsonArrayToPt(ptJson);
    ASSERT_EQ(pt.X(), 1.0);
    ASSERT_EQ(pt.Y(), 2.0);
    ASSERT_EQ(pt.Z(), 3.0);

    std::string user_ptStr = GeoDataIO::ptToStr(pt, 1);
    ASSERT_EQ(user_ptStr, "[1.0, 2.0, 3.0]");
    gp_Pnt pt2 = GeoDataIO::strToPt(user_ptStr);
    ASSERT_TRUE(pt.IsEqual(pt2, 1e-6));

    // "gp_Pnt": [1, 2, 3]
    std::string occ_ptStr = GeoDataIO::occTypeToStr<gp_Pnt>(pt);
    gp_Pnt pt3 = GeoDataIO::strToOccType<gp_Pnt>(occ_ptStr);
    EXPECT_TRUE(pt.IsEqual(pt3, 1e-6));
}
} // namespace rfgeo
