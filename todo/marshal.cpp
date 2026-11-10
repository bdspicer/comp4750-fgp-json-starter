#include "marshal.hpp"


//----------------------------------------------------------
// These are the only two functions exported by this module:

// Constructs JSON Data object corresponding to TodoList.

using Object = json::Object;
using Null = json::Null;
using Number = json::Number;
using Boolean = json::Boolean;
using String = json::String;
using Array = json::Array;
using Data = json::Data;
using MapElem = std::pair<std::string, json::Data>;

// Declarations of marshal toDoObject function
Object todoToObject(TodoList const& list);

// Declarations of unmarshal functions
//int highestId(Data const& data);
int nextIdFromData(Data const& data);
std::vector<Task> buildTasks(Data const& data);

// An object is a map of strings to Data
json::Data marshal(TodoList const& list) {
	Object toDoObject = todoToObject(list);
	Data marshaledJson = toDoObject;
	return marshaledJson;
}

// Constructs TodoList corresponding to JSON Data.  Throws std::runtime_error if the JSON Data does
// not correctly describe a to-do list.
TodoList unmarshal(json::Data const& data) {
	TodoList unData;
	unData.nextId = nextIdFromData(data);
	unData.tasks = buildTasks(data);
	return unData;
}

// You are encouraged to write additional functions to use
// as you are implementing the two above.
//----------------------------------------------------------

// Runtime error helper function
void throwRuntimeErr(std::string error) {
	throw std::runtime_error(error);
}

int nextIdFromData(Data const& data) {
	int nextId = 0;
	Object taskObj = as<Object>(data);
	auto nextIdIt = taskObj.find("nextId");
	if (nextIdIt == end(taskObj)) {
		throwRuntimeErr("Error: nextId key is missing.");
	}
	else {
		auto nextIdPair = *nextIdIt;
		Data pair = nextIdPair.second;
		nextId = as<Number>(pair);
	}
	return nextId;
}

// Converts a vector of std::string to a json::Array
Array stringVectorToArray(std::vector<std::string> stringVec) {
	Array strArray;
	for (std::string str : stringVec) {
		String strTag = str;
		strArray.push_back(strTag);
	}
	return strArray;
}

// Builds a json::Object from a Task
Object taskToObject(Task const& task) {
	Object taskObj;
	taskObj["id"] = task.id;
	taskObj["title"] = task.title;
	taskObj["done"] = task.done;
	taskObj["tags"] = stringVectorToArray(task.tags);
	if (task.completed != std::nullopt) {
		String dateCompleted = *task.completed;
		taskObj["completed"] = dateCompleted;
	}
	else {
		taskObj["completed"] = json::null;
	}
	return taskObj;
}

// Calls taskToObject on a vector of tasks
Array translateTaskList(const std::vector<Task>& tasks) {
	Array translatedTasks;
	for (const Task& task : tasks) {
		translatedTasks.push_back(taskToObject(task));
	}
	return translatedTasks;
}

// Converts a TodoList to a json::Object represenation
Object todoToObject(TodoList const& list) {
	Object listObj;
	listObj["nextId"] = list.nextId;
	listObj["tasks"] = translateTaskList(list.tasks);
	return listObj;
}

// Returns access to an Object's "tags" array
std::vector<std::string> getTags(const Array& tags) {
	std::vector<std::string> tagList = std::vector<std::string>(tags.size());
	if (tags.size() != 0) {
		for (int i = 0; i < tags.size(); ++i) {
			tagList[i] = as<String>(tags[i]);
		}
	}
	return tagList;
}

// Returns a Data's task array
Array getTasksFromData(Data const& data) {
	Object toDoObject = as<Object>(data);
	auto tasks = toDoObject.find(String{ "tasks" });
	if (tasks == toDoObject.end()) {
		throwRuntimeErr("Error: No \"tasks\" list found.");
	}
	MapElem task = *tasks;
	if (!is<Array>(task.second)) {
		throwRuntimeErr("Error: \"tasks\" value must be stored as an Array.");
	}
	return as<Array>(task.second);
}

// Builds a task using an Object, throws runtime error if key's value is missing
Task buildTask(Object taskObj) {
	Task task;
	if (is<Number>(taskObj)["id"]) {
		task.id = as<Number>(taskObj["id"]);
	}
	else {
		throwRuntimeErr("Error: Task ID must be an Int.");
	}
	if ((is<String>(taskObj)["title"])) {
		String title = as<String>(taskObj["title"]);
		task.title = title;
	}
	else {
		throwRuntimeErr("Error: Title must be a String.");
	}
	if (is<Boolean>(taskObj)["done"]) {
		task.done = as<Boolean>(taskObj["done"]);
	}
	else {
		throwRuntimeErr("Error: \"Done\" value must be a Boolean.");
	}
	if (is<Array>(taskObj)["tags"]) {
		task.tags = getTags(as<Array>(taskObj["tags"]));
	}
	else {
		throwRuntimeErr("Error: \"Tags\" must hold an Array.");
	}
	if (is<Null>(taskObj["completed"])) {
		task.completed = std::nullopt;
	}
	else if (is<String>(taskObj["completed"])) {
		task.completed = as<String>(taskObj["completed"]);
	}
	else {
		throwRuntimeErr("Error: \"Completed\" value must be Null or String value.");
	}
	return task;
}

// Calls "buildTask" on each Data object in taskArray
std::vector<Task> buildTasks(Data const& data) {
	Array taskArray = getTasksFromData(data);
	std::vector<Task> tasks = std::vector<Task>(taskArray.size());
	if (taskArray.size() != 0) {
		for (int i = 0; i < taskArray.size(); ++i) {
			tasks[i] = buildTask(as<Object>(taskArray[i]));
		}
	}
	return tasks;
}