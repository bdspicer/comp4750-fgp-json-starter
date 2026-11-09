#include "marshal.hpp"
#include "json.hpp"


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

Object todoToObject(TodoList const& list);
int highestId(Data const& data);
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
	unData.nextId = highestId(data);
	unData.tasks = buildTasks(data);
	return unData;
}


// You are encouraged to write additional functions to use
// as you are implementing the two above.
//----------------------------------------------------------

std::vector<std::string> getTags(Array tags) {
	std::vector<std::string> tagList = std::vector<std::string>(tags.size());
	if (tags.size() != 0) {
		for (int i = 0; i < tags.size(); ++i) {
			tagList[i] = as<String>(tags[i]);
		}
	}
	return tagList;
}

void throwRuntimeErr(std::string error) {
	throw std::runtime_error(error);
}

Task buildTask(Object taskObj) {
	Task task;
	if (is<Number>(taskObj)["id"]){
		task.id = as<Number>(taskObj["id"]);
	}
	else {
		throwRuntimeErr("Error: Task ID must be an Int.");
	}
	if ((is<String>(taskObj)["title"])) {
		task.title = as<String>(taskObj["title"]);
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

Array getTasksFromData(Data const& data) {
	Object toDoObject = as<Object>(data);
	Array taskArray{};
	auto tasks = toDoObject.find(String{"tasks"});
	if (tasks == toDoObject.end()) {
		throwRuntimeErr("Error: No \"tasks\" list found.");
	}
	else {
		MapElem task = *tasks;
		if (is<Array>(task.second)) {
			taskArray = as<Array>(task.second);
		}
		else {
			throwRuntimeErr("Error: \"tasks\" value must be stored as an Array.");
		}
	}
	return taskArray;
}

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

int highestId(Data const& data) {
	int highestId = 0;
	Array taskArray = getTasksFromData(data);
	auto arrayEnd = end(taskArray);
	--arrayEnd;
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

