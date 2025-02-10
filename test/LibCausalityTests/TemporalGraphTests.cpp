#include "pch.h"

#include <gtest/gtest.h>

#include "algorithms/TemporalPaths.h"
#include "core/Transformations.h"

using namespace tglib;

TEST(TemporalGraphTests, TestName)
{
	std::vector<TemporalEdge> edges;

	// u = tail, v = head, t = timestamp (availability time), tt = optional transition time

    /*
     Directed graph with 7 edges and 6 nodes:
     
            -> 2 -> 4 -> 6
          /    |    ^
         1     |    |
	      \	   v    |
            -> 3 -> 5

     */
    edges.push_back(TemporalEdge{1, 2, 1, 0});
    edges.push_back(TemporalEdge{1, 3, 1, 0});
    edges.push_back(TemporalEdge{2, 3, 1, 0});
    edges.push_back(TemporalEdge{2, 4, 1, 0});
    edges.push_back(TemporalEdge{3, 5, 1, 0});
    edges.push_back(TemporalEdge{4, 6, 1, 0});
    edges.push_back(TemporalEdge{5, 4, 1, 0});

    // Represent edges above as being ordered
	const OrderedEdgeList ordered_edge_list(10, edges, {0,1});

    // Represent as an alternative 'incident list' format
	const auto asList = to_incident_lists<TGNode>(ordered_edge_list);

    // Get the shortest path from node 1 to node 6 (should be node 1 -> 2 -> 4 -> 6)
	const auto arrivalPaths = earliest_arrival_path(asList, 1, 6, {0, 1});

    // test for expected shorted path (should be node 1 -> 2 -> 4 -> 6)
    EXPECT_EQ(arrivalPaths[0], TemporalEdge( 1,2,1,0 )); // 1 -> 2
    EXPECT_EQ(arrivalPaths[1], TemporalEdge( 2,4,1,0 )); //2 -> 4 
    EXPECT_EQ(arrivalPaths[2], TemporalEdge( 4,6,1, 0)); //4 -> 6 
}
