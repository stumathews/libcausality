#include "pch.h"
#include <gtest/gtest.h>
#include "algorithms/TemporalPaths.h"
#include "core/Transformations.h"

using namespace tglib;

TEST(TemporalGraphTests, TestName)
{
	std::vector<TemporalEdge> edges;

    edges.push_back(TemporalEdge{0, 1, 1, 1});

	const OrderedEdgeList orderedEdgeList(10, edges, {0,15});

	const auto asList = to_incident_lists<TGNode>(orderedEdgeList);

	auto arrivalPaths = earliest_arrival_path(asList, 1, 2, {1, 5});
}
