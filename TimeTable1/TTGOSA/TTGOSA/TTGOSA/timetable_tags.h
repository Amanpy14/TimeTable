#pragma once

#include <string>

/*
	{
		"Days" : 6			//days
		"SlotsPerDay" :	8	//slots_per_day
		"Resources" :		//resources
			{
				{key, value},
				{key, value}
			}
	}
*/

namespace timetable_tags {
	using namespace std;

	static const string id = "id";	//common id attribute used in many nested tags

	static const string days = "days";
	static const string slots_per_day = "slotsPerDay";
	static const string resources = "resources";
	static const string resource_groups = "resourceGroups";
	static const string events = "events";
	static const string conflicting_resources = "conflictingResources";
	static const string all_resources_busy = "noResourceAvailable";
	static const string resources_busy = "resourceBusy";

	
	namespace resource_groups_tags {
		static const string id = timetable_tags::id;
		static const string resourceIds = "resourceIds";
	}

	namespace events_tags {
		static const string id = timetable_tags::id;
		static const string required_time_slots = "slotSize";
		static const string assigned_time_slot = "assignedTimeSlot";
		static const string preassigned_resources = "preassignedResourceIds";
		static const string assigned_resources = "assignedResourceIds";
		static const string unassigned_resources = "unassignedResourceGroupIds";
	}

	namespace busy_resources_tags {
		static const string resource_id = "resourceId";
		static const string days = "days";
		static const string time_slots = "timeSlots";
	}
}
