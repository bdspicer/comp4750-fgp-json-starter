#include "marshal.hpp"
#include "json.hpp"


//----------------------------------------------------------
// These are the only two functions exported by this module:

// Constructs JSON Data object corresponding to TodoList.

using Object = json::Object;
using Number = json::Number;
using Boolean = json::Boolean;
using String = json::String;
using Array = json::Array;
using Data = json::Data;
using MapElem = std::pair<std::string, json::Data>;

Object todoToObject(TodoList const& list);
int highestId(Data& data);
Task buildTasks(Object taskObj);

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
	unData.nextId = highestId(data);
	unData.tasks = buildTasks(data);
	return unData;
}


// You are encouraged to write additional functions to use
// as you are implementing the two above.
//----------------------------------------------------------

std::vector<std::string> getTags(Array tags) {
	std::vector<std::string> tagList = std::vector<std::string>(tags.size());
	for (int i = 0; i < tags.size(); ++i) {
		tagList[i] = as<String>(tags[i]);
	}
	return tagList;
}

Task buildTask(Object taskObj) {
	Task task;
	task.id = as<Number>(taskObj["id"]);
	task.title = as<String>(taskObj["title"]);
	task.done = as<Boolean>(taskObj["done"]);
	task.tags = getTags(as<Array>(taskObj["tags"]));
	if (!is<json::Null>(taskObj["completed"])) {
		task.completed = std::nullopt;
	}
	else {
		task.completed = as<String>(taskObj["completed"]);
	}
	return task;
}

Array getTasksFromData(Data const& data) {
	Object toDoObject = as<Object>(data);
	auto tasks = std::find(begin(toDoObject), end(toDoObject), "tasks");
	MapElem task = *tasks;
	Array taskArray = as<Array>(task.second);
	return taskArray;
}

std::vector<Task> buildTasks(Data const& data) {
	Array taskArray = getTasksFromData(data);
	std::vector<Task> tasks = std::vector<Task>(taskArray.size());
	for (int i = 0; i < taskArray.size(); ++i) {
		tasks[i] = buildTask(as<Object>(taskArray[i]));
	}
	return tasks;
}

int highestId(Data const& data) {
	int highestId = 0;
	Array taskArray = getTasksFromData(data);
	auto arrayEnd = end(taskArray)--;
	Object lastArrayElem = as<Object>(*arrayEnd);
	highestId = as<Number>(lastArrayElem["id"]);
	return ++highestId;
}

Array getTags(std::vector<std::string> tags) {
	Array arr;
	for (std::string tag : tags) {
		String strTag = tag;
		arr.push_back(strTag);
	}
	return arr;
}

Object taskToObject(Task const& task) {
	Object taskObj;
	taskObj["id"] = task.id;
	taskObj["title"] = task.title;
	taskObj["done"] = task.done;
	taskObj["tags"] = getTags(task.tags);
	if (task.completed != std::nullopt) {
		String dateCompleted = *task.completed;
		taskObj["completed"] = dateCompleted;
	}
	else {
		taskObj["completed"] = json::null;
	}
	return taskObj;
}

Array translateTaskList(const std::vector<Task>& tasks) {
	Array translatedTasks;
	for (Task task : tasks) {
		translatedTasks.push_back(taskToObject(task));
	}
	return translatedTasks;
}

Object todoToObject(TodoList const& list) {
	Object listObj;
	listObj["nextId"] = list.nextId;
	listObj["tasks"] = translateTaskList(list.tasks);
	return listObj;
}

