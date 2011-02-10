//@+leo-ver=5-thin
//@+node:gcross.20110209133648.2291: * @thin codequest.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110209133648.2300: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/function.hpp>
#include <map>

#include "codequest.hpp"
//@-<< Includes >>

namespace CodeQuest {

//@+<< Usings >>
//@+node:gcross.20110209133648.2301: ** << Usings >>
using boost::assign::map_list_of;
using boost::function;

using std::map;
//@-<< Usings >>

//@+others
//@+node:gcross.20110209133648.2304: ** solver table
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

map<unsigned int,solver> solver_table = map_list_of
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
;
//@+node:gcross.20110209133648.2307: ** Functions
//@+node:gcross.20110209133648.2302: *3* function solve
code solve(const dynamic_operator_vector& operators,const bool optimize_logical_qubits) {
    unsigned int const number_of_qubits = operators[0].length();
    map<unsigned int,solver>::const_iterator solver_table_entry = solver_table.find(number_of_qubits);
    if(solver_table_entry != solver_table.end()) {
        return solver_table_entry->second(operators,optimize_logical_qubits);
    } else {
        dynamic_qec qec(operators);
        if(optimize_logical_qubits) qec.optimize_logical_qubits();
        return code(qec);
    }
}
//@+node:gcross.20110209133648.2306: *3* operator<<
ostream& operator<<(ostream& out, const code& code) { return writeYAML(out,code); }
//@-others

}
//@-leo
