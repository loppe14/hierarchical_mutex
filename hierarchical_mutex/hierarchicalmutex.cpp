#include "hierarchicalmutex.h"
thread_local uint32_t HierarchicalMutex::highiestPriority(0);
 uint32_t HierarchicalMutex::aveilablePriority()
{
	return highiestPriority + 1;
}
HierarchicalMutex::HierarchicalMutex(uint32_t  _priority/*== aveilablePriority()*/)
	:currentPriority(_priority),
	 previousPriority(0)
{}
void HierarchicalMutex::checkForViolation()
{
	if (currentPriority <= highiestPriority) throw HierarchyViolation();
}
void HierarchicalMutex::updateHighiestPriority()
{
	previousPriority = highiestPriority;
	highiestPriority = currentPriority;
}
void HierarchicalMutex::lock() 
{
	checkForViolation();
	internalMutex.lock();
	updateHighiestPriority();
}
bool HierarchicalMutex::try_lock()
{
	checkForViolation();
	if (!internalMutex.try_lock()) return false;
	updateHighiestPriority();
	return true;
}
void HierarchicalMutex::unlock()
{
	if (currentPriority != highiestPriority) throw HierarchyViolation();
	highiestPriority = previousPriority;
	internalMutex.unlock();
}
bool operator>(const HierarchicalMutex& m1, const HierarchicalMutex& m2)
{
	return m1.currentPriority > m2.currentPriority;
}
bool operator<(const HierarchicalMutex& m1, const HierarchicalMutex& m2)
{
	return m1.currentPriority < m2.currentPriority;
}