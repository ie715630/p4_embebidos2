/*
 * filters.h
 *
 *  Created on: Nov 30, 2020
 *      Author: sergio_mndz
 */

#ifndef P4_EMBEBIDOS2_FILTERS_H_
#define P4_EMBEBIDOS2_FILTERS_H_

#include "arm_math.h"
#include "stdio.h"
#include "stdint.h"

#define LENGTH_SAMPLES		1530

#define BLOCK_SIZE    		153
#define NUM_TAPS         	150

const float64_t FIR_lowPass[NUM_TAPS] = {
  -0.005107942637335911,
  -0.000882847611705671,
  -0.0009499538742000041,
  -0.001014514380327177,
  -0.001074895774335321,
  -0.0011305067583516253,
  -0.0011806303595039394,
  -0.0012237992196810893,
  -0.0012588736162363347,
  -0.001285116848544197,
  -0.0013014484264189782,
  -0.0013066030490013994,
  -0.0012996273563859807,
  -0.0012796014250752608,
  -0.0012452740179299364,
  -0.0011953829036226915,
  -0.0011290010424794866,
  -0.0010453221681074247,
  -0.0009434180780505006,
  -0.0008223369813277263,
  -0.0006810939922125789,
  -0.0005185768897312357,
  -0.00033396592548864273,
  -0.00012707761026907468,
  0.00010242802197191,
  0.0003556910455544181,
  0.0006333839956043166,
  0.0009347885917128077,
  0.0012600606317445802,
  0.0016107987523428778,
  0.0019857774024812673,
  0.0023826938938826497,
  0.0028066849384664394,
  0.003252415857090675,
  0.0037201796756511163,
  0.004211388002794668,
  0.004724961052827231,
  0.005258856092836425,
  0.005811947626667229,
  0.006383556926754679,
  0.006972593823808002,
  0.007577624511201354,
  0.008196966102765291,
  0.00882859797455908,
  0.0094705052706177,
  0.0101210717635718,
  0.01077883281626892,
  0.011442062069475204,
  0.012108866744839965,
  0.012777310529486411,
  0.013445028611601274,
  0.014109072982053737,
  0.014766549672405468,
  0.015415092730018735,
  0.01605274716404718,
  0.01667833255437091,
  0.017291559403058612,
  0.01789040079575831,
  0.01846764620487054,
  0.01901435060720276,
  0.01955298879744417,
  0.02005707551051687,
  0.020535800635206592,
  0.02098577666192252,
  0.021404645944822262,
  0.02179050869302883,
  0.022142508545310982,
  0.022459239825062715,
  0.022739037964084274,
  0.02298095379891556,
  0.023184325874593147,
  0.023348135147111197,
  0.0234714138622092,
  0.023553729124832386,
  0.023594944512707623,
  0.023594944512707623,
  0.023553729124832386,
  0.0234714138622092,
  0.023348135147111197,
  0.023184325874593147,
  0.02298095379891556,
  0.022739037964084274,
  0.022459239825062715,
  0.022142508545310982,
  0.02179050869302883,
  0.021404645944822262,
  0.02098577666192252,
  0.020535800635206592,
  0.02005707551051687,
  0.01955298879744417,
  0.01901435060720276,
  0.01846764620487054,
  0.01789040079575831,
  0.017291559403058612,
  0.01667833255437091,
  0.01605274716404718,
  0.015415092730018735,
  0.014766549672405468,
  0.014109072982053737,
  0.013445028611601274,
  0.012777310529486411,
  0.012108866744839965,
  0.011442062069475204,
  0.01077883281626892,
  0.0101210717635718,
  0.0094705052706177,
  0.00882859797455908,
  0.008196966102765291,
  0.007577624511201354,
  0.006972593823808002,
  0.006383556926754679,
  0.005811947626667229,
  0.005258856092836425,
  0.004724961052827231,
  0.004211388002794668,
  0.0037201796756511163,
  0.003252415857090675,
  0.0028066849384664394,
  0.0023826938938826497,
  0.0019857774024812673,
  0.0016107987523428778,
  0.0012600606317445802,
  0.0009347885917128077,
  0.0006333839956043166,
  0.0003556910455544181,
  0.00010242802197191,
  -0.00012707761026907468,
  -0.00033396592548864273,
  -0.0005185768897312357,
  -0.0006810939922125789,
  -0.0008223369813277263,
  -0.0009434180780505006,
  -0.0010453221681074247,
  -0.0011290010424794866,
  -0.0011953829036226915,
  -0.0012452740179299364,
  -0.0012796014250752608,
  -0.0012996273563859807,
  -0.0013066030490013994,
  -0.0013014484264189782,
  -0.001285116848544197,
  -0.0012588736162363347,
  -0.0012237992196810893,
  -0.0011806303595039394,
  -0.0011305067583516253,
  -0.001074895774335321,
  -0.001014514380327177,
  -0.0009499538742000041,
  -0.000882847611705671,
  -0.005107942637335911
};

const float64_t FIR_bandPass[NUM_TAPS] = {
  0.004084576450986212,
  0.0029644932884568824,
  0.002911226815740862,
  0.0020600906688527662,
  0.0006753715580541339,
  -0.0006940551790152767,
  -0.0014322039311009311,
  -0.0011359713813667725,
  0.00017326114684865752,
  0.002019219412057447,
  0.0036581095849587495,
  0.0044085883967283504,
  0.003989974768845906,
  0.002664893560727899,
  0.0011699949811417427,
  0.0003378517458504916,
  0.0007153961892790636,
  0.0022421039641894822,
  0.004230289178268406,
  0.0056772772674521695,
  0.00574661694477387,
  0.004207059857491861,
  0.0016100433655873922,
  -0.0009233064686508576,
  -0.0022271758584672535,
  -0.0016843035384268461,
  0.0004230387312035079,
  0.0029771814591868216,
  0.004533244471310454,
  0.00402454221112976,
  0.0013452820618494498,
  -0.002512147047045667,
  -0.00587553326903409,
  -0.007188108674974735,
  -0.005812596465561313,
  -0.002436286387908612,
  0.0011682878426915034,
  0.0029529214016351057,
  0.0016141821131824374,
  -0.002680179185312668,
  -0.008245972356241665,
  -0.012613440490712925,
  -0.013717553949630824,
  -0.010987333125344605,
  -0.005771187249294068,
  -0.00082256788691287,
  0.000973786510769934,
  -0.0019389869074306532,
  -0.008867921597589949,
  -0.017005861541386086,
  -0.022650741902945137,
  -0.022999997824272384,
  -0.017668502572795314,
  -0.009140038170888947,
  -0.0018108210473372817,
  0.0000357882059703475,
  -0.0055771675344180585,
  -0.017037172327250764,
  -0.029514036864628685,
  -0.036991342875986356,
  -0.035150440038688426,
  -0.023792860854802634,
  -0.007554846071464499,
  0.005654875178750016,
  0.008053149157455575,
  -0.004142042600600628,
  -0.02809275928817696,
  -0.05433943036826598,
  -0.06975108732841566,
  -0.062498542729948973,
  -0.027156775660635287,
  0.032179874494521976,
  0.10221778028293954,
  0.16435109826078728,
  0.20081119475390435,
  0.20081119475390435,
  0.16435109826078728,
  0.10221778028293954,
  0.032179874494521976,
  -0.027156775660635287,
  -0.062498542729948973,
  -0.06975108732841566,
  -0.05433943036826598,
  -0.02809275928817696,
  -0.004142042600600628,
  0.008053149157455575,
  0.005654875178750016,
  -0.007554846071464499,
  -0.023792860854802634,
  -0.035150440038688426,
  -0.036991342875986356,
  -0.029514036864628685,
  -0.017037172327250764,
  -0.0055771675344180585,
  0.0000357882059703475,
  -0.0018108210473372817,
  -0.009140038170888947,
  -0.017668502572795314,
  -0.022999997824272384,
  -0.022650741902945137,
  -0.017005861541386086,
  -0.008867921597589949,
  -0.0019389869074306532,
  0.000973786510769934,
  -0.00082256788691287,
  -0.005771187249294068,
  -0.010987333125344605,
  -0.013717553949630824,
  -0.012613440490712925,
  -0.008245972356241665,
  -0.002680179185312668,
  0.0016141821131824374,
  0.0029529214016351057,
  0.0011682878426915034,
  -0.002436286387908612,
  -0.005812596465561313,
  -0.007188108674974735,
  -0.00587553326903409,
  -0.002512147047045667,
  0.0013452820618494498,
  0.00402454221112976,
  0.004533244471310454,
  0.0029771814591868216,
  0.0004230387312035079,
  -0.0016843035384268461,
  -0.0022271758584672535,
  -0.0009233064686508576,
  0.0016100433655873922,
  0.004207059857491861,
  0.00574661694477387,
  0.0056772772674521695,
  0.004230289178268406,
  0.0022421039641894822,
  0.0007153961892790636,
  0.0003378517458504916,
  0.0011699949811417427,
  0.002664893560727899,
  0.003989974768845906,
  0.0044085883967283504,
  0.0036581095849587495,
  0.002019219412057447,
  0.00017326114684865752,
  -0.0011359713813667725,
  -0.0014322039311009311,
  -0.0006940551790152767,
  0.0006753715580541339,
  0.0020600906688527662,
  0.002911226815740862,
  0.0029644932884568824,
  0.004084576450986212
};

const float64_t FIR_highPass[NUM_TAPS] = {
  -0.0000023474138927870026,
  -0.000005982666319828465,
  -0.00000907075654818391,
  -0.000010708913194781192,
  -0.000006597184382697088,
  0.0000036300075894586957,
  0.000019285212924062926,
  0.00003381278248074868,
  0.00003902708798439849,
  0.000026200064094991518,
  -0.000006798615149359743,
  -0.00005283501266511183,
  -0.00009359255164328876,
  -0.00010573251639559376,
  -0.0000703740461321218,
  0.00001420723183474701,
  0.00012537556824022397,
  0.00021797508754782508,
  0.0002395256291206351,
  0.000153985290653106,
  -0.00003409363807457766,
  -0.00026833938469138127,
  -0.00045141644374096433,
  -0.0004800694863936459,
  -0.00029365053727248514,
  0.00008363105093738456,
  0.0005298067777491055,
  0.0008559485927522162,
  0.0008778639163825828,
  0.0005046776950809212,
  -0.000193890806210174,
  -0.0009793573951742106,
  -0.0015139434849623262,
  -0.0014936443268684526,
  -0.0007971048914561583,
  0.00041595280408134754,
  0.0017143038358246215,
  0.002532323904249973,
  0.0023988430666251883,
  0.0011716328685366416,
  -0.0008291320810524263,
  -0.0028703303410820722,
  -0.0040525591442271205,
  -0.003680959029398263,
  -0.0016164134303399977,
  0.001555057460283645,
  0.004645149469735907,
  0.006277649158334438,
  0.005462875609750581,
  0.0021058145833884943,
  -0.002788887942181386,
  -0.0073590863481755794,
  -0.00954636947279233,
  -0.007959923072705455,
  -0.0026018359932273934,
  0.004884148348088334,
  0.011628875081937896,
  0.014553596837806701,
  0.011654479243751518,
  0.0030583708730757245,
  -0.008631289359688876,
  -0.01896186434304879,
  -0.02313904383961864,
  -0.01795293623750289,
  -0.0034277564400613768,
  0.016489231647532316,
  0.03460589740934539,
  0.04247128212638922,
  0.03311730066379739,
  0.003668412281637173,
  -0.04310737425189769,
  -0.09893849065850646,
  -0.15197167000329892,
  -0.18996070058362102,
  0.7962480144236175,
  -0.18996070058362102,
  -0.15197167000329892,
  -0.09893849065850646,
  -0.04310737425189769,
  0.003668412281637173,
  0.03311730066379739,
  0.04247128212638922,
  0.03460589740934539,
  0.016489231647532316,
  -0.0034277564400613768,
  -0.01795293623750289,
  -0.02313904383961864,
  -0.01896186434304879,
  -0.008631289359688876,
  0.0030583708730757245,
  0.011654479243751518,
  0.014553596837806701,
  0.011628875081937896,
  0.004884148348088334,
  -0.0026018359932273934,
  -0.007959923072705455,
  -0.00954636947279233,
  -0.0073590863481755794,
  -0.002788887942181386,
  0.0021058145833884943,
  0.005462875609750581,
  0.006277649158334438,
  0.004645149469735907,
  0.001555057460283645,
  -0.0016164134303399977,
  -0.003680959029398263,
  -0.0040525591442271205,
  -0.0028703303410820722,
  -0.0008291320810524263,
  0.0011716328685366416,
  0.0023988430666251883,
  0.002532323904249973,
  0.0017143038358246215,
  0.00041595280408134754,
  -0.0007971048914561583,
  -0.0014936443268684526,
  -0.0015139434849623262,
  -0.0009793573951742106,
  -0.000193890806210174,
  0.0005046776950809212,
  0.0008778639163825828,
  0.0008559485927522162,
  0.0005298067777491055,
  0.00008363105093738456,
  -0.00029365053727248514,
  -0.0004800694863936459,
  -0.00045141644374096433,
  -0.00026833938469138127,
  -0.00003409363807457766,
  0.000153985290653106,
  0.0002395256291206351,
  0.00021797508754782508,
  0.00012537556824022397,
  0.00001420723183474701,
  -0.0000703740461321218,
  -0.00010573251639559376,
  -0.00009359255164328876,
  -0.00005283501266511183,
  -0.000006798615149359743,
  0.000026200064094991518,
  0.00003902708798439849,
  0.00003381278248074868,
  0.000019285212924062926,
  0.0000036300075894586957,
  -0.000006597184382697088,
  -0.000010708913194781192,
  -0.00000907075654818391,
  -0.000005982666319828465,
  -0.0000023474138927870026
  -0.0000023474138927870026
};

void initialize_fir(arm_fir_instance_f32* S, float32_t* coeffs, float32_t* state);

void call_fir(arm_fir_instance_f32* S, float32_t* pts_src, float32_t* ptr_dst);

#endif /* P4_EMBEBIDOS2_FILTERS_H_ */
