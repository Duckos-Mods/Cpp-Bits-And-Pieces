#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>

class EventBus
{
public:
	EventBus() {};

	// Add an event listener for a specific event name and function
	template <typename ClassType>
	void addEventListener(std::function<void(ClassType*)> function, ClassType* instance, std::string eventName, bool loop = false)
	{
		auto Wrapper = [function = std::move(function), instance]() { function(instance); };
		auto emd = eventMetaData(Wrapper, loop);


		pushBackEventListener(DataPack(eventName, { loop, Wrapper }));
	}

	// Overloaded version of addEventListener for functions in a class and with args
	template <typename ClassType, typename ...Args>
	void addEventListener(std::function<void(ClassType*, Args...)> function, ClassType* instance, std::string eventName, Args... args, bool loop = false)
	{
		auto Wrapper = [function = std::move(function), instance, args]() { function(instance, args...); };
		auto emd = eventMetaData(Wrapper, loop);


		pushBackEventListener(DataPack(eventName, { loop, Wrapper }));
	}


	// Overloaded version to add an event listener with variable arguments
	template <typename ...Args>
	void addEventListener(std::function<void(Args...)> function, std::string eventName, Args... args, bool loop = false)
	{
		// Define a lambda function Wrapper that captures function and args and calls function with args when invoked
		auto Wrapper = [function = std::move(function), args]() { function(args...); };

		// Create an eventMetaData object emd with the Wrapper lambda function and the loop parameter
		auto emd = eventMetaData(Wrapper, loop);

		pushBackEventListener(DataPack(eventName, { loop, Wrapper }));
	}

	// Overloaded version of addEventListener for functions without a Class
	void addEventListener(std::function<void()> function, std::string eventName, bool loop = false);

	// Fire an event by its name
	__forceinline void fireEvent(std::string eventName)
	{
		_allWaitingEvents.push_back(eventName);
	}

	// Forcefully fire an event without waiting in the event queue
	// Should be used sparingly and with caution
	__forceinline void fireEventForce(std::string eventName)
	{
		searchForEventAndFire(eventName);
	}

	// Process all events waiting in the event queue
	void tick();

public:

	// Inner class to store event metadata
	class eventMetaData
	{
	public:
		// Constructor that adds a function to the functionList with an associated loop flag
		eventMetaData(std::function<void()> func, bool loop)
		{
			functionList.insert({ index, { loop, func } });
			index++;
		}

		// Calls all the functions in the functionList and handles removal of functions marked as non-looping
		inline bool callAllFunctions()
		{
			// Iterate over the functionList using structured binding
			for (auto& [HashedIndex, functData] : functionList)
			{
				// Invoke the stored function
				functData.second();

				// If the function is marked as non-looping, remove it from the functionList
				if (functData.first == false)
				{
					functionList.erase(HashedIndex);
				}
			}

			// Rehash the functionList to optimize its internal structure
			functionList.rehash(functionList.size());

			// Handle the case when all functions have been removed
			if (functionList.empty())
			{
				return false;
			}

			// Return true if there are remaining functions in the functionList
			return true;
		}

		__forceinline void addFunct(std::function<void()> func, bool loop)
		{

			functionList.insert({ index, { loop, func } });
			index++;
		}


		__forceinline void addFunct(std::pair<bool, std::function<void()>> data)
		{

			functionList.insert({ index, data });
			index++;
		}

	public:

	private:
		std::size_t index = 0;

		std::unordered_map<std::size_t, std::pair<bool, std::function<void()>>> functionList;
	};



	class DataPack
	{
	public:
		DataPack(std::string Index, std::pair<bool, std::function<void()>> functs): index(Index), functPair(functs) {};

		std::string index;

		std::pair<bool, std::function<void()>> functPair;

	};

private:

	void pushBackEventListener(DataPack dataPair);
	__forceinline void searchForEventAndFire(std::string eventName)
	{
		// Search for the eventName in the _eventListeners unordered_map
		auto hashMapOperator = _eventListeners.find(eventName);

		// If the eventName is not found, return from the function
		if (hashMapOperator == _eventListeners.end()) {
			return;
		}

		// Get a reference to the eventMetaData associated with the eventName
		auto& eventMetaData = hashMapOperator->second;

		// Invoke the stored function in the eventMetaData
		if (!eventMetaData.callAllFunctions())
		{
			_eventListeners.erase(eventName);
		}

		// Return from the function
		return;
	}

private:


	// A map to store event listeners, where the key is the event name and the value is the metadata
	std::unordered_map<std::string, eventMetaData> _eventListeners;

	// A vector to store events waiting to be processed
	std::vector<std::string> _allWaitingEvents;
};

