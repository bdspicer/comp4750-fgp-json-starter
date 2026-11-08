#include "marshal.hpp"
#include "json.hpp"


//----------------------------------------------------------
// These are the only two functions exported by this module:

// Constructs JSON Data object corresponding to TodoList.

using Object = json::Object;
using Number = json::Number;
using String = json::String;
using Array = json::Array;
using Data = json::Data;

// An object is a map of strings to Data
json::Data marshal(TodoList const& list) {
	json::Object toDoObject = todoToObject(list);
  return { toDoObject };
}


// Constructs TodoList corresponding to JSON Data.  Throws std::runtime_error if the JSON Data does
// not correctly describe a to-do list.
TodoList unmarshal(json::Data const& data) {
	TodoList unData;
	unData.nextId = data.variant()["nextId"];
  return { };
}


// You are encouraged to write additional functions to use
// as you are implementing the two above.
//----------------------------------------------------------
Object todoToObject(TodoList const& list){
	Object listObj;
	listObj["nextId"] = list.nextId;
	listObj["tasks"] = translateTaskList(list.tasks);
	return listObj;
}

Array translateTaskList(std::vector<Task> tasks) {
	Array translatedTasks;
	for (Task task : tasks) {
		translatedTasks.push_back(taskToObject(task));
	}
}

Object taskToObject(Task task) {
	Object taskObj;
	taskObj["id"] = task.id;
	taskObj["title"] = task.title;
	taskObj["done"] = task.done;
	taskObj["tags"] = getTags(task.tags);
	if (task.completed) {
		String dateCompleted = *task.completed;
		taskObj["completed"] = dateCompleted;
	}
	else {
		taskObj["completed"] = json::null;
	}
}

Array getTags(std::vector<std::string> tags){
	Array arr;
	for (std::string tag : tags) {
		String strTag = tag;
		arr.push_back(strTag);
	}
	return arr;
}

