// Includes {{{
#include <boost/assign/list_of.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <map>

#include "codequest.hpp"
// }}}

namespace CodeQuest {

// Namespace imports {{{
using boost::assign::map_list_of;
using boost::function;
using boost::none;
using boost::optional;

using std::map;
// }}}

// Typedefs {{{
typedef function<code (const dynamic_operator_vector&,bool)> solver;

extern solver solver_001;
extern solver solver_002;
extern solver solver_003;
extern solver solver_004;
extern solver solver_005;
extern solver solver_006;
extern solver solver_007;
extern solver solver_008;
extern solver solver_009;
extern solver solver_010;
extern solver solver_011;
extern solver solver_012;
extern solver solver_013;
extern solver solver_014;
extern solver solver_015;
extern solver solver_016;
extern solver solver_017;
extern solver solver_018;
extern solver solver_019;
extern solver solver_020;
extern solver solver_021;
extern solver solver_022;
extern solver solver_023;
extern solver solver_024;
extern solver solver_025;
extern solver solver_026;
extern solver solver_027;
extern solver solver_028;
extern solver solver_029;
extern solver solver_030;
extern solver solver_031;
extern solver solver_032;
extern solver solver_033;
extern solver solver_034;
extern solver solver_035;
extern solver solver_036;
extern solver solver_037;
extern solver solver_038;
extern solver solver_039;
extern solver solver_040;
extern solver solver_041;
extern solver solver_042;
extern solver solver_043;
extern solver solver_044;
extern solver solver_045;
extern solver solver_046;
extern solver solver_047;
extern solver solver_048;
extern solver solver_049;
extern solver solver_050;
extern solver solver_051;
extern solver solver_052;
extern solver solver_053;
extern solver solver_054;
extern solver solver_055;
extern solver solver_056;
extern solver solver_057;
extern solver solver_058;
extern solver solver_059;
extern solver solver_060;
extern solver solver_061;
extern solver solver_062;
extern solver solver_063;
extern solver solver_064;
extern solver solver_065;
extern solver solver_066;
extern solver solver_067;
extern solver solver_068;
extern solver solver_069;
extern solver solver_070;
extern solver solver_071;
extern solver solver_072;
extern solver solver_073;
extern solver solver_074;
extern solver solver_075;
extern solver solver_076;
extern solver solver_077;
extern solver solver_078;
extern solver solver_079;
extern solver solver_080;
extern solver solver_081;
extern solver solver_082;
extern solver solver_083;
extern solver solver_084;
extern solver solver_085;
extern solver solver_086;
extern solver solver_087;
extern solver solver_088;
extern solver solver_089;
extern solver solver_090;
extern solver solver_091;
extern solver solver_092;
extern solver solver_093;
extern solver solver_094;
extern solver solver_095;
extern solver solver_096;
extern solver solver_097;
extern solver solver_098;
extern solver solver_099;
extern solver solver_100;
extern solver solver_101;
extern solver solver_102;
extern solver solver_103;
extern solver solver_104;
extern solver solver_105;
extern solver solver_106;
extern solver solver_107;
extern solver solver_108;
extern solver solver_109;
extern solver solver_110;
extern solver solver_111;
extern solver solver_112;
extern solver solver_113;
extern solver solver_114;
extern solver solver_115;
extern solver solver_116;
extern solver solver_117;
extern solver solver_118;
extern solver solver_119;
extern solver solver_120;
extern solver solver_121;
extern solver solver_122;
extern solver solver_123;
extern solver solver_124;
extern solver solver_125;
extern solver solver_126;
extern solver solver_127;
extern solver solver_128;
extern solver solver_129;
extern solver solver_130;
extern solver solver_131;
extern solver solver_132;
extern solver solver_133;
extern solver solver_134;
extern solver solver_135;
extern solver solver_136;
extern solver solver_137;
extern solver solver_138;
extern solver solver_139;
extern solver solver_140;
extern solver solver_141;
extern solver solver_142;
extern solver solver_143;
extern solver solver_144;
extern solver solver_145;
extern solver solver_146;
extern solver solver_147;
extern solver solver_148;
extern solver solver_149;
extern solver solver_150;
extern solver solver_151;
extern solver solver_152;
extern solver solver_153;
extern solver solver_154;
extern solver solver_155;
extern solver solver_156;
extern solver solver_157;
extern solver solver_158;
extern solver solver_159;
extern solver solver_160;
extern solver solver_161;
extern solver solver_162;
extern solver solver_163;
extern solver solver_164;
extern solver solver_165;
extern solver solver_166;
extern solver solver_167;
extern solver solver_168;
extern solver solver_169;
extern solver solver_170;
extern solver solver_171;
extern solver solver_172;
extern solver solver_173;
extern solver solver_174;
extern solver solver_175;
extern solver solver_176;
extern solver solver_177;
extern solver solver_178;
extern solver solver_179;
extern solver solver_180;
extern solver solver_181;
extern solver solver_182;
extern solver solver_183;
extern solver solver_184;
extern solver solver_185;
extern solver solver_186;
extern solver solver_187;
extern solver solver_188;
extern solver solver_189;
extern solver solver_190;
extern solver solver_191;
extern solver solver_192;
extern solver solver_193;
extern solver solver_194;
extern solver solver_195;
extern solver solver_196;
extern solver solver_197;
extern solver solver_198;
extern solver solver_199;
extern solver solver_200;
extern solver solver_201;
extern solver solver_202;
extern solver solver_203;
extern solver solver_204;
extern solver solver_205;
extern solver solver_206;
extern solver solver_207;
extern solver solver_208;
extern solver solver_209;
extern solver solver_210;
extern solver solver_211;
extern solver solver_212;
extern solver solver_213;
extern solver solver_214;
extern solver solver_215;
extern solver solver_216;
extern solver solver_217;
extern solver solver_218;
extern solver solver_219;
extern solver solver_220;
extern solver solver_221;
extern solver solver_222;
extern solver solver_223;
extern solver solver_224;
extern solver solver_225;
extern solver solver_226;
extern solver solver_227;
extern solver solver_228;
extern solver solver_229;
extern solver solver_230;
extern solver solver_231;
extern solver solver_232;
extern solver solver_233;
extern solver solver_234;
extern solver solver_235;
extern solver solver_236;
extern solver solver_237;
extern solver solver_238;
extern solver solver_239;
extern solver solver_240;
extern solver solver_241;
extern solver solver_242;
extern solver solver_243;
extern solver solver_244;
extern solver solver_245;
extern solver solver_246;
extern solver solver_247;
extern solver solver_248;
extern solver solver_249;
extern solver solver_250;
extern solver solver_251;
extern solver solver_252;
extern solver solver_253;
extern solver solver_254;
extern solver solver_255;
extern solver solver_256;
// }}}

// Functions {{{
optional<solver> getSolverFor(unsigned int const number_of_qubits) { // {{{
    static map<unsigned int,solver> solver_table = map_list_of
        (  1,solver_001)
        (  2,solver_002)
        (  3,solver_003)
        (  4,solver_004)
        (  5,solver_005)
        (  6,solver_006)
        (  7,solver_007)
        (  8,solver_008)
        (  9,solver_009)
        ( 10,solver_010)
        ( 11,solver_011)
        ( 12,solver_012)
        ( 13,solver_013)
        ( 14,solver_014)
        ( 15,solver_015)
        ( 16,solver_016)
        ( 17,solver_017)
        ( 18,solver_018)
        ( 19,solver_019)
        ( 20,solver_020)
        ( 21,solver_021)
        ( 22,solver_022)
        ( 23,solver_023)
        ( 24,solver_024)
        ( 25,solver_025)
        ( 26,solver_026)
        ( 27,solver_027)
        ( 28,solver_028)
        ( 29,solver_029)
        ( 30,solver_030)
        ( 31,solver_031)
        ( 32,solver_032)
        ( 33,solver_033)
        ( 34,solver_034)
        ( 35,solver_035)
        ( 36,solver_036)
        ( 37,solver_037)
        ( 38,solver_038)
        ( 39,solver_039)
        ( 40,solver_040)
        ( 41,solver_041)
        ( 42,solver_042)
        ( 43,solver_043)
        ( 44,solver_044)
        ( 45,solver_045)
        ( 46,solver_046)
        ( 47,solver_047)
        ( 48,solver_048)
        ( 49,solver_049)
        ( 50,solver_050)
        ( 51,solver_051)
        ( 52,solver_052)
        ( 53,solver_053)
        ( 54,solver_054)
        ( 55,solver_055)
        ( 56,solver_056)
        ( 57,solver_057)
        ( 58,solver_058)
        ( 59,solver_059)
        ( 60,solver_060)
        ( 61,solver_061)
        ( 62,solver_062)
        ( 63,solver_063)
        ( 64,solver_064)
        ( 65,solver_065)
        ( 66,solver_066)
        ( 67,solver_067)
        ( 68,solver_068)
        ( 69,solver_069)
        ( 70,solver_070)
        ( 71,solver_071)
        ( 72,solver_072)
        ( 73,solver_073)
        ( 74,solver_074)
        ( 75,solver_075)
        ( 76,solver_076)
        ( 77,solver_077)
        ( 78,solver_078)
        ( 79,solver_079)
        ( 80,solver_080)
        ( 81,solver_081)
        ( 82,solver_082)
        ( 83,solver_083)
        ( 84,solver_084)
        ( 85,solver_085)
        ( 86,solver_086)
        ( 87,solver_087)
        ( 88,solver_088)
        ( 89,solver_089)
        ( 90,solver_090)
        ( 91,solver_091)
        ( 92,solver_092)
        ( 93,solver_093)
        ( 94,solver_094)
        ( 95,solver_095)
        ( 96,solver_096)
        ( 97,solver_097)
        ( 98,solver_098)
        ( 99,solver_099)
        (100,solver_100)
        (101,solver_101)
        (102,solver_102)
        (103,solver_103)
        (104,solver_104)
        (105,solver_105)
        (106,solver_106)
        (107,solver_107)
        (108,solver_108)
        (109,solver_109)
        (110,solver_110)
        (111,solver_111)
        (112,solver_112)
        (113,solver_113)
        (114,solver_114)
        (115,solver_115)
        (116,solver_116)
        (117,solver_117)
        (118,solver_118)
        (119,solver_119)
        (120,solver_120)
        (121,solver_121)
        (122,solver_122)
        (123,solver_123)
        (124,solver_124)
        (125,solver_125)
        (126,solver_126)
        (127,solver_127)
        (128,solver_128)
        (129,solver_129)
        (130,solver_130)
        (131,solver_131)
        (132,solver_132)
        (133,solver_133)
        (134,solver_134)
        (135,solver_135)
        (136,solver_136)
        (137,solver_137)
        (138,solver_138)
        (139,solver_139)
        (140,solver_140)
        (141,solver_141)
        (142,solver_142)
        (143,solver_143)
        (144,solver_144)
        (145,solver_145)
        (146,solver_146)
        (147,solver_147)
        (148,solver_148)
        (149,solver_149)
        (150,solver_150)
        (151,solver_151)
        (152,solver_152)
        (153,solver_153)
        (154,solver_154)
        (155,solver_155)
        (156,solver_156)
        (157,solver_157)
        (158,solver_158)
        (159,solver_159)
        (160,solver_160)
        (161,solver_161)
        (162,solver_162)
        (163,solver_163)
        (164,solver_164)
        (165,solver_165)
        (166,solver_166)
        (167,solver_167)
        (168,solver_168)
        (169,solver_169)
        (170,solver_170)
        (171,solver_171)
        (172,solver_172)
        (173,solver_173)
        (174,solver_174)
        (175,solver_175)
        (176,solver_176)
        (177,solver_177)
        (178,solver_178)
        (179,solver_179)
        (180,solver_180)
        (181,solver_181)
        (182,solver_182)
        (183,solver_183)
        (184,solver_184)
        (185,solver_185)
        (186,solver_186)
        (187,solver_187)
        (188,solver_188)
        (189,solver_189)
        (190,solver_190)
        (191,solver_191)
        (192,solver_192)
        (193,solver_193)
        (194,solver_194)
        (195,solver_195)
        (196,solver_196)
        (197,solver_197)
        (198,solver_198)
        (199,solver_199)
        (200,solver_200)
        (201,solver_201)
        (202,solver_202)
        (203,solver_203)
        (204,solver_204)
        (205,solver_205)
        (206,solver_206)
        (207,solver_207)
        (208,solver_208)
        (209,solver_209)
        (210,solver_210)
        (211,solver_211)
        (212,solver_212)
        (213,solver_213)
        (214,solver_214)
        (215,solver_215)
        (216,solver_216)
        (217,solver_217)
        (218,solver_218)
        (219,solver_219)
        (220,solver_220)
        (221,solver_221)
        (222,solver_222)
        (223,solver_223)
        (224,solver_224)
        (225,solver_225)
        (226,solver_226)
        (227,solver_227)
        (228,solver_228)
        (229,solver_229)
        (230,solver_230)
        (231,solver_231)
        (232,solver_232)
        (233,solver_233)
        (234,solver_234)
        (235,solver_235)
        (236,solver_236)
        (237,solver_237)
        (238,solver_238)
        (239,solver_239)
        (240,solver_240)
        (241,solver_241)
        (242,solver_242)
        (243,solver_243)
        (244,solver_244)
        (245,solver_245)
        (246,solver_246)
        (247,solver_247)
        (248,solver_248)
        (249,solver_249)
        (250,solver_250)
        (251,solver_251)
        (252,solver_252)
        (253,solver_253)
        (254,solver_254)
        (255,solver_255)
        (256,solver_256)
    ;
    map<unsigned int,solver>::const_iterator solver_table_entry = solver_table.find(number_of_qubits);
    return (solver_table_entry != solver_table.end())
            ? make_optional(solver_table_entry->second)
            : none
    ;
}
// }}}

code solve(const dynamic_operator_vector& operators,const bool optimize_logical_qubits) { // {{{
    unsigned int const number_of_qubits = operators[0].length();
    optional<solver> maybe_solver = getSolverFor(number_of_qubits);
    if(maybe_solver) {
        return (*maybe_solver)(operators,optimize_logical_qubits);
    } else {
        dynamic_qec qec(operators);
        if(optimize_logical_qubits) qec.optimize_logical_qubits();
        return code(qec);
    }
}
// }}}

ostream& operator<<(ostream& out, const code& code) { return writeYAML(out,code); }
// }}}

}
