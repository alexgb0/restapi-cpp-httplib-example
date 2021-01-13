#include "httplib/httplib.hpp"
#include "json/json.hpp"

#include <vector>
#include <algorithm>
#include <map>


bool bValidReq(nlohmann::json json, std::vector<std::string> keys)
{
	std::vector<bool> vTest;

	for (size_t i = 0; i < keys.size(); i++)
	{
		if (!json[keys[i]].empty())
			vTest.push_back(true);
		else
			vTest.push_back(false);
	}

	std::vector<bool>::iterator it = std::find(vTest.begin(), vTest.end(), false);

	if (it != vTest.end())
		return false;
	else
		return true;
}

int main(void)
{
	httplib::Server svr;

	std::map<size_t, std::string> db;
	size_t id = 0;

	svr.Get("/", [](const httplib::Request&, httplib::Response& res)
		{
			res.set_content("Hello World!", "text/plain");
		});

	svr.Get(R"(/msg/(\d+))", [&](const httplib::Request& req, httplib::Response& res)
		{
			auto n = req.matches[1];

			nlohmann::json jRes;

			try
			{
				jRes["id"] = std::stoi(n);
				jRes["msg"] = db[std::stoi(n)];

				res.set_content(jRes.dump(), "application/json");
			}
			catch (const std::exception&)
			{
				res.set_content("Cannot find the requested message.", "text/plain");
			}

		});

	svr.Post("/", [&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json json = nlohmann::json::parse(req.body);

			if (bValidReq(json, { "msg" }))
			{
				db.insert({ id, json["msg"] });

				nlohmann::json jRes;

				jRes["id"] = id;
				jRes["msg"] = db[id];

				id++;

				res.set_content(jRes.dump(), "application/json");
			}
			else
				res.set_content("The request doesn't meet the requirements", "text/plain");

		});

	svr.Patch("/", [&](const httplib::Request& req, httplib::Response& res)
		{
			nlohmann::json json = nlohmann::json::parse(req.body);

			if (bValidReq(json, { "id", "msg" }))
			{
				db[json["id"]] = json["msg"];

				nlohmann::json jRes;

				jRes["id"] = json["id"];
				jRes["msg"] = db[json["id"]];

				res.set_content(jRes.dump(), "application/json");
			}
			else
				res.set_content("The request doesn't meet the requirements", "text/plain");

		});

	svr.Delete(R"(/msg/(\d+))", [&](const httplib::Request& req, httplib::Response& res)
		{

			auto n = req.matches[1];

			db[std::stoi(n)] = "deleted";

			nlohmann::json jRes;

			jRes["id"] = std::stoi(n);
			jRes["msg"] = "deleted";

			res.set_content(jRes.dump(), "application/json");

		});


	svr.listen("0.0.0.0", 8080);
	return 0;
}