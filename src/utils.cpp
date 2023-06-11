#include "utils.hpp"
#include "bseq.hpp"
#include <math.h>
#include <string>

double CHAR_DISTRIBUTION[128] = {0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0.019578060965172565,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0.167564443682168,
                                 0,
                                 0.0015754276887500987,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0.0015078622753204398,
                                 0.0003307916441739124,
                                 0.0003314254660634964,
                                 0,
                                 0,
                                 0.008634492219614468,
                                 0.002076717421222119,
                                 0.011055184780313847,
                                 0.000519607185080999,
                                 0.005918945715880591,
                                 0.004937789430804492,
                                 0.002756237869045172,
                                 0.0021865587546870337,
                                 0.0018385271551164353,
                                 0.0025269211093936652,
                                 0.0019199098857390264,
                                 0.0018243295447897528,
                                 0.002552781042488694,
                                 0.002442242504945237,
                                 0.00012036277683200988,
                                 0,
                                 0.00044107665296153596,
                                 0,
                                 0.0004404428310719519,
                                 0,
                                 0,
                                 0.0024774830020061096,
                                 0.0017387002075069484,
                                 0.002987392712176473,
                                 0.0010927723198318497,
                                 0.0012938206232079082,
                                 0.001220297284016159,
                                 0.0009310209736100016,
                                 0.0008752446473266058,
                                 0.0020910417959267183,
                                 0.0008814561018445294,
                                 0.0003808001912620934,
                                 0.0010044809306127922,
                                 0.0018134911904778657,
                                 0.0012758834637326799,
                                 0.0008210528757671701,
                                 0.00138908405321239,
                                 0.00010001709417636208,
                                 0.0011037374385216535,
                                 0.0030896915651553373,
                                 0.0030701064687671904,
                                 0.0010426370083657518,
                                 0.0002556203680692448,
                                 0.0008048270353938186,
                                 0,
                                 0.00025194420110965734,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0.0612553996079051,
                                 0.01034644514338097,
                                 0.02500268898936656,
                                 0.03188948073064199,
                                 0.08610229517681191,
                                 0.015750347191785568,
                                 0.012804659959943725,
                                 0.02619237267611581,
                                 0.05480626188138746,
                                 0.000617596049210692,
                                 0.004945712204424292,
                                 0.03218192615049607,
                                 0.018140172626462205,
                                 0.05503703643138501,
                                 0.0541904405334676,
                                 0.017362092874808832,
                                 0.00100853739070613,
                                 0.051525029341199825,
                                 0.0518864979648296,
                                 0.0632964962389326,
                                 0.019247776378510318,
                                 0.007819143740853554,
                                 0.009565830104169261,
                                 0.0023064144740073764,
                                 0.010893686962847832,
                                 0.0005762708620098124,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0};

double score(std::string s) {
    int len = s.length();
    double freqs[128]{0};
    for (char c : s) {
        if (c > 127) {
            continue;
        }
        if (c < 0) {
            throw "cannot calculate score of invalid string";
        }
        freqs[c] += ((double)1) / len;
    }
    double bc = 0;
    for (int i = 0; i < 127; i++) {
        bc += sqrt(CHAR_DISTRIBUTION[i] * freqs[i]);
    }
    return bc;
}

// int hammingDistance(hex hx1, hex hx2) {
//     if (hx1.raw.length() != hx2.raw.length())
//         throw "cannot compute hamming distance between strings of unequal "
//               "length";
//     hex hx = hexor(hx1, hx2);
//     int len = hx.raw.length();
//     int numOnes = 0;
//     for (int i = 0; i < len; i++) {
//         int val = std::stoi(hx.raw.substr(i, 1), 0, 16);
//         while (val) {
//             numOnes += val & 1;
//             val >>= 1;
//         }
//     }
//     return numOnes;
// }
//
// int hammingDistance(std::string s1, std::string s2) {
//     try {
//         return hammingDistance(toHex(s1), toHex(s2));
//     } catch (const char *msg) {
//         std::cout << msg << std::endl;
//         return -1;
//     }
// }