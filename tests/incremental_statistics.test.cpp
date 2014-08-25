#include <thread>

#include <gtest/gtest.h>

#include <handystats/incremental_statistics.hpp>

#include <handystats/chrono.hpp>

class IncrementalStatisticsTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		opts = handystats::config::incremental_statistics();
	}
	virtual void TearDown() {
	}

	handystats::incremental_statistics stats;
	handystats::config::incremental_statistics opts;
};

TEST_F(IncrementalStatisticsTest, TestIntervalCount) {
	opts.moving_interval = std::chrono::milliseconds(1);

	stats = handystats::incremental_statistics(opts);

	ASSERT_NEAR(stats.interval_count(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int step = 0; step < STEP_COUNT; ++step) {
			stats(step);
		}

		ASSERT_NEAR(stats.interval_count(), STEP_COUNT, 0.05 * STEP_COUNT);
	}
}

TEST_F(IncrementalStatisticsTest, TestIntervalSum) {
	opts.moving_interval = std::chrono::milliseconds(1);

	stats = handystats::incremental_statistics(opts);

	ASSERT_NEAR(stats.interval_sum(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int step = 0; step < STEP_COUNT; ++step) {
			stats(1);
		}

		ASSERT_NEAR(stats.interval_sum(), STEP_COUNT, 0.05 * STEP_COUNT);
	}
}

TEST_F(IncrementalStatisticsTest, TestIntervalMean) {
	opts.moving_interval = std::chrono::milliseconds(1);

	stats = handystats::incremental_statistics(opts);

	ASSERT_NEAR(stats.interval_mean(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		handystats::incremental_statistics mean_stats;

		for (int step = 0; step < STEP_COUNT; ++step) {
			auto value = rand() % 1000;
			stats(value);
			mean_stats(value);
		}

		ASSERT_NEAR(stats.interval_mean(), mean_stats.mean(), 0.05 * mean_stats.mean());
	}
}

TEST_F(IncrementalStatisticsTest, TestMovingAverageConstruction) {
	opts.moving_average_alpha = 0.66;

	stats = handystats::incremental_statistics(opts);

	ASSERT_EQ(stats.count(), 0);
	ASSERT_NEAR(stats.moving_average(), 0.0, 1E-8);
}

TEST_F(IncrementalStatisticsTest, AverageOfSingleEntryEqualToItsValue) {
	opts.moving_average_alpha = 0.66;

	stats = handystats::incremental_statistics(opts);

	stats(10);

	ASSERT_NEAR(stats.moving_average(), 10, 1E-8);
}

TEST_F(IncrementalStatisticsTest, AverageOfUnitElementsEqual1) {
	opts.moving_average_alpha = 0.66;

	stats = handystats::incremental_statistics(opts);

	const int STEP_COUNT = 100;
	for (int step = 0; step < STEP_COUNT; ++step) {
		stats(1);
	}

	ASSERT_NEAR(stats.moving_average(), 1, 1E-8);
}

TEST_F(IncrementalStatisticsTest, AverageWithAlpha1EqualLastElement) {
	opts.moving_average_alpha = 1.0;

	stats = handystats::incremental_statistics(opts);

	stats(100);
	ASSERT_NEAR(stats.moving_average(), 100, 1E-8);

	stats(1);
	ASSERT_NEAR(stats.moving_average(), 1, 1E-8);

	stats(-200);
	ASSERT_NEAR(stats.moving_average(), -200, 1E-8);
}

TEST_F(IncrementalStatisticsTest, AverageWithAlpha0EqualFirstElement) {
	opts.moving_average_alpha = 0.0;

	stats = handystats::incremental_statistics(opts);

	stats(100);
	ASSERT_NEAR(stats.moving_average(), 100, 1E-8);

	stats(1);
	ASSERT_NEAR(stats.moving_average(), 100, 1E-8);

	stats(-200);
	ASSERT_NEAR(stats.moving_average(), 100, 1E-8);
}
