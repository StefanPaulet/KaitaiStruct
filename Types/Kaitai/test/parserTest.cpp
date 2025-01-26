//
// Created by stefan on 1/24/2025.
//

#include "utils/ParserMock.hpp"
#include <gtest/gtest.h>
#include <sstream>

namespace {
using namespace kaitai::testing;
using kaitai::detail::Parser;
}

TEST(ParserTest, MetaEndianTest) {
  using enum kaitai::detail::Endian;
  std::stringstream s1 {R"(meta:
  endian: le
seq:
types:
)"};
  Parser p1 {s1};
  auto res1 = p1();
  ASSERT_EQ(res1.getMeta().endian, LITTLE);

  std::stringstream s2 {R"(meta:
  endian: be
seq:
types:
)"};
  Parser p2 {s2};
  auto res2 = p2();
  ASSERT_EQ(res2.getMeta().endian, BIG);
}

TEST(ParserTest, MetaIdTest) {
  std::stringstream s1 {R"(meta:
  id: myFile
seq:
types:
)"};
  Parser p1 {s1};
  auto res1 = p1();
  ASSERT_EQ(res1.getMeta().id, "myFile");
}

TEST(ParserTest, MetaFileExtensionTest) {
  std::stringstream s1 {R"(meta:
  file-extension: jpg
seq:
types:
)"};
  Parser p1 {s1};
  auto res1 = p1();
  ASSERT_EQ(res1.getMeta().fileExtension, "jpg");
}

TEST(ParserTest, MainSeqTest) {
  std::stringstream s1 {R"(meta:
seq:
  - id: t1
    type: myType1
  - id: t2
    type: myType2
types:
)"};
  Parser p1 {s1};
  auto res1 = p1();
  ASSERT_TRUE(compareSequences(res1.getSequence(), {{"t1", "myType1"}, {"t2", "myType2"}}));
}

TEST(ParserTest, TypesTest) {
  std::stringstream s1 {R"(meta:
seq:
  - id: t1
    type: t1
types:
  t1:
    seq:
      - id: t1Content
        contents: 'T1'
)"};
  Parser p1 {s1};
  auto res1 = p1();
  ASSERT_TRUE(
    compareTypes(
      res1.getTypes(),
      {
        {
          "t1",
          {contents("t1Content", "T1")}
        }
  }));


  std::stringstream s2 {R"(meta:
seq:
  - id: t2
    type: t2
types:
  t2:
    seq:
      - id: t2Size
        size: 2
)"};
  Parser p2 {s2};
  auto res2 = p2();
  ASSERT_TRUE(
    compareTypes(
      res2.getTypes(),
      {
        {
          "t2",
          {size("t2Size", 2)}
        }
  }));


  std::stringstream s3 {R"(meta:
seq:
  - id: t3
    type: t3
types:
  t3:
    seq:
      - id: t3Type1
        type: u1
      - id: t3Type2
        type: u2
)"};
  Parser p3 {s3};
  auto res3 = p3();
  ASSERT_TRUE(
    compareTypes(
      res1.getTypes(),
      {
        {
          "t3",
          {type("t3Type1", Chunk::RawType::U1), type("t3Type2", Chunk::RawType::U2)}
        }
  }));
}

TEST(ParserTest, Overall) {
  using enum kaitai::detail::Endian;

  std::stringstream s{R"(meta:
  id: gif
  file-extension: gif
  endian: le
seq:
  - id: header
    type: header
  - id: logical_screen
    type: logical_screen
types:
  header:
    seq:
      - id: magic
        contents: 'GIF'
      - id: version
        size: 3
  logical_screen:
    seq:
      - id: image_width
        type: u2
      - id: image_height
        type: u2
      - id: flags
        type: u1
      - id: bg_color_index
        type: u1
      - id: pixel_aspect_ratio
        type: u1
)"};
  Parser p{s};
  auto res = p();
  ASSERT_EQ(res.getMeta().id, "gif");
  ASSERT_EQ(res.getMeta().fileExtension, "gif");
  ASSERT_EQ(res.getMeta().endian, LITTLE);

  ASSERT_TRUE(compareSequences(res.getSequence(), {{"header", "header"}, {"logical_screen", "logical_screen"}}));

  using enum Chunk::RawType;
  ASSERT_TRUE(
    compareTypes(
      res.getTypes(),
      {
        {"header", {contents("magic", "GIF"), size("version", 3)}},
        {"logical_screen", {
          type("image_width", U2), type("image_height", U2), type("flags", U1), type("bg_color_index", U1),
          type("pixel_aspect_ratio", U1)
        }}
      }
    )
  );
}