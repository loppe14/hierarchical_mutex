#pragma once
#include <mutex>
#include <string>
class HierarchyViolation;
class HierarchicalMutex
{
	static thread_local uint32_t  highiestPriority;
	const uint32_t  currentPriority;
	uint32_t  previousPriority;
	std::mutex internalMutex;

	void updateHighiestPriority();
	void checkForViolation();

public:
	static uint32_t aveilablePriority();
	explicit HierarchicalMutex(uint32_t  _priority= aveilablePriority());
	void lock();
	bool try_lock();
	void unlock();
	friend bool operator>(const HierarchicalMutex &m1, const HierarchicalMutex &m2);
	friend bool operator<(const HierarchicalMutex &m1, const HierarchicalMutex& m2);

};
class HierarchyViolation :public std::exception
{
public:
	const char* what() const noexcept
	{
		return "Hierarchy violation in mutex";
	}
};