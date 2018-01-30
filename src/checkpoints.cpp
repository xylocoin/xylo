// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
    (     0, uint256("0x00006898b824b2c7d6e8bb648a2a0d6c10db1c487823d9675d790e0ab764da0b") )
    (     500, uint256("0x79f208c1bff9ee297a1555a0414d45df18fead3149cf9ac432375f26628a3a2a") )
    (     5000, uint256("0x690c7f4d8ba791b410c9a27e35589e97bfbe1eab26353286020402994648a9b5") )
    (     50000, uint256("0xdae74d0297d1e1f580a786e8825a00647cc202a26106ca1656c68fe7d7a263d2") )
    (     87000, uint256("0x14f11356eedf75692ccfe8a8710a5953c46a9039777ccaeaa7e340026d2a7dfe") )
    (     90000, uint256("0x8ec564d90a249490cc12a54dd88657607864d0bd354e6a633f9143604bf69293") )
    (     100000, uint256("0x80a1f3f49405975e3013beec54d7e9990ee9712f8140e0d43fede39aa39abb00") )
    (     105000, uint256("0xea46517cc02e803376c0517ad4cb34896e2e7ab322597113bc2eb6dda17a55c3") )
    (     150000, uint256("0xfc0ac99bd20f8f1b8213c0ecb17d695f9bc2bbfd4cee1fbd56fd9f4e26f2147d") )
    (     200000, uint256("0x2d795789def425927b0e44228f508c37ecda88ad029424c87db41d75250e2339") )
    (     250000, uint256("0x19bbba86466cc00cbd221fdf3bfcbf4a1b70ed46523a24ed0ccad17cfd93470a") )
    (     300000, uint256("0x42d07c66362c99aa5d9bd400e51d62eca81085d2556b6adedc4cd437e38602f2") )
    (     350000, uint256("0x34c3eeec4447038df491b80fa8c464706eea146b11919e97caafc5224b89c129") )
    (     400000, uint256("0xc99286a2ff482b34c9a9b950b6bca833edb46db293ab477a49f135816386368b") )
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
