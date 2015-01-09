#ifndef STATS_H_
#define STATS_H_

#include <atomic>

class Stats {
public:
	Stats();

	void incClientConnections();
	unsigned long long clientConnections() const;

private:
	std::atomic_ullong clientConns;
};

inline Stats::Stats() : clientConns(0) {
}

inline void Stats::incClientConnections() {
	clientConns.fetch_add(1);
}

inline unsigned long long Stats::clientConnections() const {
	return clientConns.load();
}

extern Stats stats;

#endif
