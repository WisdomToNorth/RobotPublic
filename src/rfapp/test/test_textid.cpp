
#include <gtest/gtest.h>

#include "rfapp/textid.h"

namespace rfapp
{
class TestTextId
{
    RF_DECLARE_TEXT_ID_FUNCTIONS(rfapp::TestTextId)
public:
    TestTextId() = default;
    ~TestTextId() = default;
};

TEST(test_app, test_textid1)
{
    TestTextId textId;
    std::string_view context = textId.textIdContext();
    EXPECT_EQ(context, "rfapp::TestTextId");

    TextId id = textId.textId("test_textid1");
    std::string_view key = id.key;
    EXPECT_EQ(key, "test_textid1");

    std::string_view tr = textId.textIdTr("test_textid1");
    EXPECT_EQ(tr, "test_textid1");
}
} // namespace rfapp
