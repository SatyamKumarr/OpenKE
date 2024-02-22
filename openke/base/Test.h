#ifndef TEST_H
#define TEST_H
#include "Setting.h"
#include "Reader.h"
#include "Corrupt.h"

/*=====================================================================================
link prediction
======================================================================================*/
INT lastHead = 0;
INT lastTail = 0;
INT lastRel = 0;
REAL l1_filter_tot = 0, l1_tot = 0, r1_tot = 0, r1_filter_tot = 0, l_tot = 0, r_tot = 0, l_filter_rank = 0, l_rank = 0, l_filter_reci_rank = 0, l_reci_rank = 0,l_map=0,lmap_filtered_tot=0,l_map1=0,lmap1_filtered_tot=0;
REAL l3_filter_tot = 0, l3_tot = 0, r3_tot = 0, r3_filter_tot = 0, l_filter_tot = 0, r_filter_tot = 0, r_filter_rank = 0, r_rank = 0, r_filter_reci_rank = 0, r_reci_rank = 0,r_map=0,rmap_filtered_tot=0,r_map1=0,rmap1_filtered_tot=0;
REAL rel3_tot = 0, rel3_filter_tot = 0, rel_filter_tot = 0, rel_filter_rank = 0, rel_rank = 0, rel_filter_reci_rank = 0, rel_reci_rank = 0, rel_tot = 0, rel1_tot = 0, rel1_filter_tot = 0,relmap_filter=0,relmap_filter_tot=0,relmap1_filter=0,relmap1_filter_tot=0;;

REAL l1_filter_tot_constrain = 0, l1_tot_constrain = 0, r1_tot_constrain = 0, r1_filter_tot_constrain = 0, l_tot_constrain = 0, r_tot_constrain = 0, l_filter_rank_constrain = 0, l_rank_constrain = 0, l_filter_reci_rank_constrain = 0, l_reci_rank_constrain = 0,lmap_filter_tot_constrain = 0, lmap_tot_constrain = 0;
REAL l3_filter_tot_constrain = 0, l3_tot_constrain = 0, r3_tot_constrain = 0, r3_filter_tot_constrain = 0, l_filter_tot_constrain = 0, r_filter_tot_constrain = 0, r_filter_rank_constrain = 0, r_rank_constrain = 0, r_filter_reci_rank_constrain = 0, r_reci_rank_constrain = 0,rmap_filter_tot_constrain = 0, rmap_tot_constrain = 0;
REAL hit1, hit3, hit10, mr, mrr,map;
REAL hit1TC, hit3TC, hit10TC, mrTC, mrrTC,mapTC;

extern "C"
void initTest() {
    lastHead = 0;
    lastTail = 0;
    lastRel = 0;
    l1_filter_tot = 0, l1_tot = 0, r1_tot = 0, r1_filter_tot = 0, l_tot = 0, r_tot = 0, l_filter_rank = 0, l_rank = 0, l_filter_reci_rank = 0, l_reci_rank = 0,l_map=0,lmap_filtered_tot=0;
    l3_filter_tot = 0, l3_tot = 0, r3_tot = 0, r3_filter_tot = 0, l_filter_tot = 0, r_filter_tot = 0, r_filter_rank = 0, r_rank = 0, r_filter_reci_rank = 0, r_reci_rank = 0,r_map=0,rmap_filtered_tot=0;
    REAL rel3_tot = 0, rel3_filter_tot = 0, rel_filter_tot = 0, rel_filter_rank = 0, rel_rank = 0, rel_filter_reci_rank = 0, rel_reci_rank = 0, rel_tot = 0, rel1_tot = 0, rel1_filter_tot = 0,relmap_filter=0,relmap_filter_tot=0;

    l1_filter_tot_constrain = 0, l1_tot_constrain = 0, r1_tot_constrain = 0, r1_filter_tot_constrain = 0, l_tot_constrain = 0, r_tot_constrain = 0, l_filter_rank_constrain = 0, l_rank_constrain = 0, l_filter_reci_rank_constrain = 0, l_reci_rank_constrain = 0,lmap_filter_tot_constrain = 0, lmap_tot_constrain = 0;
    l3_filter_tot_constrain = 0, l3_tot_constrain = 0, r3_tot_constrain = 0, r3_filter_tot_constrain = 0, l_filter_tot_constrain = 0, r_filter_tot_constrain = 0, r_filter_rank_constrain = 0, r_rank_constrain = 0, r_filter_reci_rank_constrain = 0, r_reci_rank_constrain = 0,rmap_filter_tot_constrain = 0, rmap_tot_constrain = 0;
}

extern "C"
void getHeadBatch(INT *ph, INT *pt, INT *pr) {
    for (INT i = 0; i < entityTotal; i++) {
        ph[i] = i;
        pt[i] = testList[lastHead].t;
        pr[i] = testList[lastHead].r;
    }
    lastHead++;
}

extern "C"
void getTailBatch(INT *ph, INT *pt, INT *pr) {
    for (INT i = 0; i < entityTotal; i++) {
        ph[i] = testList[lastTail].h;
        pt[i] = i;
        pr[i] = testList[lastTail].r;
    }
    lastTail++;
}

extern "C"
void getRelBatch(INT *ph, INT *pt, INT *pr) {
    for (INT i = 0; i < relationTotal; i++) {
        ph[i] = testList[lastRel].h;
        pt[i] = testList[lastRel].t;
        pr[i] = i;
    }
}

extern "C"
void testHead(REAL *con, INT lastHead, bool type_constrain = false) {
    INT h = testList[lastHead].h;
    INT t = testList[lastHead].t;
    INT r = testList[lastHead].r;
    INT lef, rig;
    if (type_constrain) {
        lef = head_lef[r];
        rig = head_rig[r];
    }
    REAL minimal = con[h];
    INT l_s = 0;
    INT l_filter_s = 0;
    INT l_filter_prec = 0;
    INT l_s_constrain = 0;
    INT l_filter_s_constrain = 0;
    INT l_prec=0;
    

    for (INT j = 0; j < entityTotal; j++) {
        if (j != h) {
            REAL value = con[j];
            if (value < minimal) {
                l_s += 1;
                l_prec+=1;
                if (not _find(j, t, r))
                    l_filter_s += 1;
                    l_filter_prec += 1;
            }
            if (type_constrain) {
                while (lef < rig && head_type[lef] < j) lef ++;
                if (lef < rig && j == head_type[lef]) {
                    if (value < minimal) {
                        l_s_constrain += 1;
                        if (not _find(j, t, r)) {
                            l_filter_s_constrain += 1;
                        }
                    }  
                }
            }
        }
    }

    if (l_filter_s < 10) l_filter_tot += 1;
    if (l_filter_prec < 10) lmap_filtered_tot += 1;
    if (l_s < 10) l_tot += 1;
    if (l_prec < 10) l_map += 1;
    if (l_filter_s < 3) l3_filter_tot += 1;
    if (l_s < 3) l3_tot += 1;
    if (l_filter_s < 1) l1_filter_tot += 1;
    if (l_filter_prec < 1) lmap1_filtered_tot += 1;
    if (l_s < 1) l1_tot += 1;
    if (l_prec < 1) l_map1 += 1;

    l_filter_rank += (l_filter_s+1);
    l_rank += (1 + l_s);
    l_filter_reci_rank += 1.0/(l_filter_s+1);
    l_reci_rank += 1.0/(l_s+1);



    if (type_constrain) {
        if (l_filter_s_constrain < 10) l_filter_tot_constrain += 1;
        if (l_s_constrain < 10) l_tot_constrain += 1;
        if (l_filter_s_constrain < 3) l3_filter_tot_constrain += 1;
        if (l_s_constrain < 3) l3_tot_constrain += 1;
        if (l_filter_s_constrain < 1) l1_filter_tot_constrain += 1;
        if (l_s_constrain < 1) l1_tot_constrain += 1;

        l_filter_rank_constrain += (l_filter_s_constrain+1);
        l_rank_constrain += (1+l_s_constrain);
        l_filter_reci_rank_constrain += 1.0/(l_filter_s_constrain+1);
        l_reci_rank_constrain += 1.0/(l_s_constrain+1);
    }
}

extern "C"
void testTail(REAL *con, INT lastTail, bool type_constrain = false) {
    INT h = testList[lastTail].h;
    INT t = testList[lastTail].t;
    INT r = testList[lastTail].r;
    INT lef, rig;
    if (type_constrain) {
        lef = tail_lef[r];
        rig = tail_rig[r];
    }
    REAL minimal = con[t];
    INT r_s = 0;INT r_prec=0;
    INT r_filter_s = 0;
    INT r_filter_prec = 0;
    INT r_s_constrain = 0;
    INT r_filter_s_constrain = 0;
    for (INT j = 0; j < entityTotal; j++) {
        if (j != t) {
            REAL value = con[j];
            if (value < minimal) {
                r_s += 1;
                r_prec+=1;
                if (not _find(h, j, r))
                    r_filter_s += 1;
                    r_filter_prec +=1;
            }
            if (type_constrain) {
                while (lef < rig && tail_type[lef] < j) lef ++;
                if (lef < rig && j == tail_type[lef]) {
                        if (value < minimal) {
                            r_s_constrain += 1;
                            if (not _find(h, j ,r)) {
                                r_filter_s_constrain += 1;
                            }
                        }
                }
            }
        }
        
    }

    if (r_filter_s < 10) r_filter_tot += 1;
    if (r_filter_prec < 10) rmap_filtered_tot += 1;
    if (r_s < 10) r_tot += 1;
    if (r_prec < 10) r_map += 1;
    if (r_filter_s < 3) r3_filter_tot += 1;
    if (r_s < 3) l3_tot += 1;
    if (r_filter_s < 1) r1_filter_tot += 1;
    if (r_filter_prec < 1) rmap1_filtered_tot += 1;
    if (r_s < 1) r1_tot += 1;
    if (r_prec < 1) r_map1 += 1;

    r_filter_rank += (1+r_filter_s);
    r_rank += (1+r_s);
    r_filter_reci_rank += 1.0/(1+r_filter_s);
    r_reci_rank += 1.0/(1+r_s);
    
    if (type_constrain) {
        if (r_filter_s_constrain < 10) r_filter_tot_constrain += 1;
        if (r_s_constrain < 10) r_tot_constrain += 1;
        if (r_filter_s_constrain < 3) r3_filter_tot_constrain += 1;
        if (r_s_constrain < 3) r3_tot_constrain += 1;
        if (r_filter_s_constrain < 1) r1_filter_tot_constrain += 1;
        if (r_s_constrain < 1) r1_tot_constrain += 1;

        r_filter_rank_constrain += (1+r_filter_s_constrain);
        r_rank_constrain += (1+r_s_constrain);
        r_filter_reci_rank_constrain += 1.0/(1+r_filter_s_constrain);
        r_reci_rank_constrain += 1.0/(1+r_s_constrain);
    }
}

extern "C"
void testRel(REAL *con) {
    INT h = testList[lastRel].h;
    INT t = testList[lastRel].t;
    INT r = testList[lastRel].r;

    REAL minimal = con[r];
    INT rel_s = 0;INT rel_prec=0;
    INT rel_filter_s = 0;INT rel_filter_prec=0;

    for (INT j = 0; j < relationTotal; j++) {
        if (j != r) {
            REAL value = con[j];
            if (value < minimal) {
                rel_s += 1;
                rel_prec += 1;
                if (not _find(h, t, j))
                    rel_filter_s += 1;
                    rel_filter_prec+=1;
            }
        }
    }

    if (rel_filter_s < 10) rel_filter_tot += 1;
    if (rel_filter_prec < 10) relmap_filter += 1;
    if (rel_s < 10) rel_tot += 1;
    if (rel_prec < 10) relmap_filter_tot += 1;
    if (rel_filter_s < 3) rel3_filter_tot += 1;
    if (rel_s < 3) rel3_tot += 1;
    if (rel_filter_s < 1) rel1_filter_tot += 1;
    if (rel_filter_prec < 1) relmap1_filter += 1;
    if (rel_s < 1) rel1_tot += 1;
    if (rel_prec < 1) relmap1_filter_tot += 1;
    

    rel_filter_rank += (rel_filter_s+1);
    rel_rank += (1+rel_s);
    rel_filter_reci_rank += 1.0/(rel_filter_s+1);
    rel_reci_rank += 1.0/(rel_s+1);

    lastRel++;
}


extern "C"
void test_link_prediction(bool type_constrain = false) {
    l_rank /= testTotal;
    r_rank /= testTotal;
    l_reci_rank /= testTotal;
    r_reci_rank /= testTotal;

 
    l_tot /= testTotal;
    l_map /= testTotal;
    l_map1 /= testTotal;
    l3_tot /= testTotal;
    l1_tot /= testTotal;
 
    r_tot /= testTotal;
    r_map /= testTotal;
    r_map1 /= testTotal;
    r3_tot /= testTotal;
    r1_tot /= testTotal;



    // with filter
    l_filter_rank /= testTotal;
    r_filter_rank /= testTotal;
    l_filter_reci_rank /= testTotal;
    r_filter_reci_rank /= testTotal;
 
    l_filter_tot /= testTotal;
    l3_filter_tot /= testTotal;
    l1_filter_tot /= testTotal;
 
    r_filter_tot /= testTotal;
    r3_filter_tot /= testTotal;
    r1_filter_tot /= testTotal;

    printf("no type constraint results:\n");
    
    printf("metric:\t\t\t MRR \t\t MAP@10 \t\t hit@10 \t\t hit@1 \t\t MAP@1\n");
    printf("Head:\t\t\t %.4f \t\t %.4f \t\t %.4f \t\t %.4f \t\t %.4f \n", l_reci_rank, l_map, l_tot, l1_tot,l_map1);
    printf("Tail:\t\t\t %.4f \t\t %.4f \t\t %.4f \t\t %.4f \t\t %.4f \n", r_reci_rank, r_map, r_tot, r1_tot,r_map1);
    // printf("averaged:\t\t %.4f \t %.4f \t %.4f \t %.4f \n",
            // (l_reci_rank+r_reci_rank)/2, (l_map+r_map)/2, (l_tot+r_tot)/2, (l1_tot+r1_tot)/2);
    printf("\n");
    // printf("l(filter):\t\t %f \t %f \t %f \t %f \t %f \n", l_filter_reci_rank, l_filter_rank, l_filter_tot, l3_filter_tot, l1_filter_tot);
    // printf("r(filter):\t\t %f \t %f \t %f \t %f \t %f \n", r_filter_reci_rank, r_filter_rank, r_filter_tot, r3_filter_tot, r1_filter_tot);
    // printf("averaged(filter):\t %f \t %f \t %f \t %f \t %f \n",
    //         (l_filter_reci_rank+r_filter_reci_rank)/2, (l_filter_rank+r_filter_rank)/2, (l_filter_tot+r_filter_tot)/2, (l3_filter_tot+r3_filter_tot)/2, (l1_filter_tot+r1_filter_tot)/2);

    //mrr = (l_filter_reci_rank+r_filter_reci_rank) / 2;
    // mr = (l_filter_rank+r_filter_rank) / 2;
    // hit10 = (l_filter_tot+r_filter_tot) / 2;
    // hit3 = (l3_filter_tot+r3_filter_tot) / 2;
    // hit1 = (l1_filter_tot+r1_filter_tot) / 2;

    if (type_constrain) {
        //type constrain
        // l_rank_constrain /= testTotal;
        // r_rank_constrain /= testTotal;
        l_reci_rank_constrain /= testTotal;
        r_reci_rank_constrain /= testTotal;
     
        l_tot_constrain /= testTotal;
        // l3_tot_constrain /= testTotal;
        l1_tot_constrain /= testTotal;
     
        r_tot_constrain /= testTotal;
        // r3_tot_constrain /= testTotal;
        r1_tot_constrain /= testTotal;

        // with filter
        l_filter_rank_constrain /= testTotal;
        r_filter_rank_constrain /= testTotal;
        l_filter_reci_rank_constrain /= testTotal;
        r_filter_reci_rank_constrain /= testTotal;
     
        l_filter_tot_constrain /= testTotal;
        l3_filter_tot_constrain /= testTotal;
        l1_filter_tot_constrain /= testTotal;
     
        r_filter_tot_constrain /= testTotal;
        r3_filter_tot_constrain /= testTotal;
        r1_filter_tot_constrain /= testTotal;

        printf("type constraint results:\n");
        
        // printf("metric:\t\t\t MRR \t\t MR \t\t hit@10 \t hit@3  \t hit@1 \n");
        // printf("l(raw):\t\t\t %f \t %f \t %f \t %f \t %f \n", l_reci_rank_constrain, l_rank_constrain, l_tot_constrain, l3_tot_constrain, l1_tot_constrain);
        // printf("r(raw):\t\t\t %f \t %f \t %f \t %f \t %f \n", r_reci_rank_constrain, r_rank_constrain, r_tot_constrain, r3_tot_constrain, r1_tot_constrain);
        // printf("averaged(raw):\t\t %f \t %f \t %f \t %f \t %f \n",
        //         (l_reci_rank_constrain+r_reci_rank_constrain)/2, (l_rank_constrain+r_rank_constrain)/2, (l_tot_constrain+r_tot_constrain)/2, (l3_tot_constrain+r3_tot_constrain)/2, (l1_tot_constrain+r1_tot_constrain)/2);
        // printf("\n");
        // printf("l(filter):\t\t %f \t %f \t %f \t %f \t %f \n", l_filter_reci_rank_constrain, l_filter_rank_constrain, l_filter_tot_constrain, l3_filter_tot_constrain, l1_filter_tot_constrain);
        // printf("r(filter):\t\t %f \t %f \t %f \t %f \t %f \n", r_filter_reci_rank_constrain, r_filter_rank_constrain, r_filter_tot_constrain, r3_filter_tot_constrain, r1_filter_tot_constrain);
        // printf("averaged(filter):\t %f \t %f \t %f \t %f \t %f \n",
        //         (l_filter_reci_rank_constrain+r_filter_reci_rank_constrain)/2, (l_filter_rank_constrain+r_filter_rank_constrain)/2, (l_filter_tot_constrain+r_filter_tot_constrain)/2, (l3_filter_tot_constrain+r3_filter_tot_constrain)/2, (l1_filter_tot_constrain+r1_filter_tot_constrain)/2);
        printf("metric:\t\t\t MRR  \t hit@10  \t hit@1 \n");
        printf("Head:\t\t\t %f \t %f \t %f \n", l_reci_rank_constrain, l_tot_constrain, l1_tot_constrain);
        printf("Tail:\t\t\t %f \t %f \t %f \n", r_reci_rank_constrain, r_tot_constrain, r1_tot_constrain);
        printf("averaged:\t\t %f \t %f \t %f \n",
                (l_reci_rank_constrain+r_reci_rank_constrain)/2, (l_tot_constrain+r_tot_constrain)/2, (l1_tot_constrain+r1_tot_constrain)/2);
        printf("\n");
        // printf("l(filter):\t\t %f \t %f \t %f \t %f \t %f \n", l_filter_reci_rank, l_filter_rank, l_filter_tot, l3_filter_tot, l1_filter_tot);
        // printf("r(filter):\t\t %f \t %f \t %f \t %f \t %f \n", r_filter_reci_rank, r_filter_rank, r_filter_tot, r3_filter_tot, r1_filter_tot);
        // printf("averaged(filter):\t %f \t %f \t %f \t %f \t %f \n",
        //         (l_filter_reci_rank+r_filter_reci_rank)/2, (l_filter_rank+r_filter_rank)/2, (l_filter_tot+r_filter_tot)/2, (l3_filter_tot+r3_filter_tot)/2, (l1_filter_tot+r1_filter_tot)/2);

        // mrrTC = (l_filter_reci_rank_constrain+r_filter_reci_rank_constrain)/2;
        //mrTC = (l_filter_rank_constrain+r_filter_rank_constrain) / 2;
        // hit10TC = (l_filter_tot_constrain+r_filter_tot_constrain) / 2;
        //hit3TC = (l3_filter_tot_constrain+r3_filter_tot_constrain) / 2;
        // hit1TC = (l1_filter_tot_constrain+r1_filter_tot_constrain) / 2;
    }
}

extern "C"
// void test_relation_prediction() {
//    // rel_rank /= testTotal;
//     rel_reci_rank /= testTotal;
//     relmap_filter=0,relmap_filter_tot=0;
//     rel_tot /= testTotal;
//     //rel3_tot /= testTotal;
//     rel1_tot /= testTotal;

//     // with filter
//     //rel_filter_rank /= testTotal;
//     rel_filter_reci_rank /= testTotal;
  
//     rel_filter_tot /= testTotal;
//     //rel3_filter_tot /= testTotal;
//     rel1_filter_tot /= testTotal;

//     printf("no type constraint results:\n");
//     printf("metric:\t\t\t MRR \t\t MAP@10 \t\t hit@10 \t\t hit@1 \n");
//     printf("Averaged:\t\t\t %.4f \t\t %.4f \t\t %.4f \t\t %.4f  \n", rel_reci_rank, rel_map, rel_tot, rel1_tot);
//     // printf("Tail:\t\t\t %.4f \t\t %.4f \t\t %.4f \t\t %.4f  \n", rel_reci_rank, r_map, r_tot, r1_tot);
//     // printf("metric:\t\t\t MRR \t\ hit@10   \t hit@1 \n");
//     // printf("averaged(raw):\t\t %f  \t %f \t %f \n",
//     //         rel_reci_rank,  rel_tot,  rel1_tot);
//     // printf("\n");
//     // printf("averaged(filter):\t %f  \t %f \t %f \n",
//     //         rel_filter_reci_rank, rel_filter_tot, rel1_filter_tot);
//     // printf("metric:\t\t\t MRR  \t hit@10  \t hit@1 \n");
//     // printf("l(raw):\t\t\t %f \t %f \t %f \n", rel_reci_rank, l_tot, l1_tot);
//     // printf("r(raw):\t\t\t %f \t %f \t %f \n", r_reci_rank, r_tot, r1_tot);
//     // printf("averaged(raw):\t\t %f \t %f \t %f \n",
//     //         (l_reci_rank+r_reci_rank)/2, (l_tot+r_tot)/2, (l1_tot+r1_tot)/2);
//     // printf("\n");
//     // printf("l(filter):\t\t %f \t %f \t %f \t %f \t %f \n", l_filter_reci_rank, l_filter_rank, l_filter_tot, l3_filter_tot, l1_filter_tot);
//     // printf("r(filter):\t\t %f \t %f \t %f \t %f \t %f \n", r_filter_reci_rank, r_filter_rank, r_filter_tot, r3_filter_tot, r1_filter_tot);
//     // printf("averaged(filter):\t %f \t %f \t %f \t %f \t %f \n",
//     //         (l_filter_reci_rank+r_filter_reci_rank)/2, (l_filter_rank+r_filter_rank)/2, (l_filter_tot+r_filter_tot)/2, (l3_filter_tot+r3_filter_tot)/2, (l1_filter_tot+r1_filter_tot)/2);

// }

extern "C"
REAL getTestLinkHit10(bool type_constrain = false) {
    if (type_constrain)
        return hit10TC;
    // printf("Retrieval MAP (Head): %f\n", hit10);
    return hit10;
}

// extern "C"
// REAL  getTestLinkHit3(bool type_constrain = false) {
//     if (type_constrain)
//         return hit3TC;
//     return hit3;
// }

extern "C"
REAL  getTestLinkHit1(bool type_constrain = false) {
    if (type_constrain)
        return hit1TC;
    // printf("Retrieval MAP (Tail): %f\n", hit1);    
    return hit1;
}

// extern "C"
// REAL  getTestLinkMR(bool type_constrain = false) {
//     if (type_constrain)
//         return mrTC;
//     return mr;
// }

extern "C"
REAL  getTestLinkMRR(bool type_constrain = false) {
    if (type_constrain)
        return mrrTC;    
    return mrr;
}


/*=====================================================================================
triple classification
======================================================================================*/
Triple *negTestList = NULL;

extern "C"
void getNegTest() {
    if (negTestList == NULL)
        negTestList = (Triple *)calloc(testTotal, sizeof(Triple));
    for (INT i = 0; i < testTotal; i++) {
        negTestList[i] = testList[i];
        if (randd(0) % 1000 < 500)
            negTestList[i].t = corrupt_head(0, testList[i].h, testList[i].r);
        else
            negTestList[i].h = corrupt_tail(0, testList[i].t, testList[i].r);
    }
}

extern "C"
void getTestBatch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr) {
    getNegTest();
    for (INT i = 0; i < testTotal; i++) {
        ph[i] = testList[i].h;
        pt[i] = testList[i].t;
        pr[i] = testList[i].r;
        nh[i] = negTestList[i].h;
        nt[i] = negTestList[i].t;
        nr[i] = negTestList[i].r;
    }
}
#endif
