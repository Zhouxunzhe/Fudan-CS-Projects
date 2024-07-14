#include "handlers.h"

#include <vector>

#include "rendering.h"

bserv::db_relation_to_object user{
	bserv::make_db_field<std::string>("sid"),
	bserv::make_db_field<std::string>("name"),
	bserv::make_db_field<std::string>("password"),
	bserv::make_db_field<bool>("is_superuser"),
	bserv::make_db_field<std::string>("major"),
	bserv::make_db_field<std::string>("phone"),
	bserv::make_db_field<std::string>("bid"),
	bserv::make_db_field<std::string>("situation"),
	bserv::make_db_field<bool>("is_active")
};

bserv::db_relation_to_object building{
	bserv::make_db_field<std::string>("bid"),
	bserv::make_db_field<int>("count")
};

bserv::db_relation_to_object inspection{
	bserv::make_db_field<std::string>("sid"),
	bserv::make_db_field<std::string>("iid"),
	bserv::make_db_field<std::string>("inspect_date"),
	bserv::make_db_field<std::string>("is_inspected"),
	bserv::make_db_field<std::string>("result")
};

bserv::db_relation_to_object inspector{
	bserv::make_db_field<std::string>("inspect_date"),
	bserv::make_db_field<std::string>("iid"),
	bserv::make_db_field<int>("count")
};

std::optional<boost::json::object> get_user_sid(
	bserv::db_transaction& tx,
	const boost::json::string& sid) {
	bserv::db_result r = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid", sid);
	lginfo << r.query(); // this is how you log info
	return user.convert_to_optional(r);
}
std::optional<boost::json::object> get_inspect_sid_date(
	bserv::db_transaction& tx,
	const boost::json::string& sid,
	const boost::json::string& inspect_date) {
	bserv::db_result r = tx.exec(
		"select sid, iid, inspect_date, is_inspected, result from inspection "
		"where sid = ? and inspect_date = ?", sid, inspect_date);
	lginfo << r.query(); // this is how you log info
	return inspection.convert_to_optional(r);
}
std::optional<boost::json::object> get_inspect_iid_date(
	bserv::db_transaction& tx,
	const boost::json::string& iid,
	const boost::json::string& inspect_date) {
	bserv::db_result r = tx.exec(
		"select inspect_date, iid, count(distinct sid) from inspection "
		"where iid = ? and inspect_date = ? and is_inspected = 'NOT' "
		"group by inspect_date, iid "
		"having count(distinct sid) > 4", iid, inspect_date);
	lginfo << r.query(); // this is how you log info
	return inspector.convert_to_optional(r);
}


std::string get_or_empty(
	boost::json::object& obj,
	const std::string& key) {
	return obj.count(key) ? obj[key].as_string().c_str() : "";
}

std::nullopt_t hello(
	bserv::response_type& response,
	std::shared_ptr<bserv::session_type> session_ptr) {
	bserv::session_type& session = *session_ptr;
	boost::json::object obj;
	if (session.count("user")) {
		if (!session.count("count")) {
			session["count"] = 0;
		}
		auto& user = session["user"].as_object();
		session["count"] = session["count"].as_int64() + 1;
		obj = {
			{"welcome", user["sid"]},
			{"count", session["count"]}
		};
	}
	else {
		obj = { {"msg", "hello, world!"} };
	}
	// the response body is a string,
	// so the `obj` should be serialized
	response.body() = boost::json::serialize(obj);
	response.prepare_payload(); // this line is important!
	return std::nullopt;
}


boost::json::object user_register(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	if (params.count("sid") == 0) {
		return {
			{"success", false},
			{"message", "`sid` is required"}
		};
	}
	if (params["sid"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`sid` is null"}
		};
	}
	if (params["name"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`name` is null"}
		};
	}
	if (params["major"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`major` is null"}
		};
	}
	if (params["phone"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`phone` is null"}
		};
	}
	if (params["situation"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`situation` is null"}
		};
	}
	if (params["bid"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`bid` is null"}
		};
	}
	if (params["password"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`password` is null"}
		};
	}
	if (params.count("password") == 0) {
		return {
			{"success", false},
			{"message", "`password` is required"}
		};
	}
	auto sid = params["sid"].as_string();
	bserv::db_transaction tx{ conn };
	auto opt_user = get_user_sid(tx, sid);
	if (opt_user.has_value()) {
		return {
			{"success", false},
			{"message", "`sid` existed"}
		};
	}
	if (params["situation"].as_string() != "positive" && 
		params["situation"].as_string() != "negative") {
		return {
			{"success", false},
			{"message", "situation is not valid"}
		};
	}
	auto password = params["password"].as_string();
	bserv::db_result r = tx.exec(
		"insert into ? "
		"(?, name, password, is_superuser, "
		"major, phone, situation, is_active) values "
		"(?, ?, ?, ?, ?, ?, ?, ?)", bserv::db_name("student"),
		bserv::db_name("sid"), sid,
		get_or_empty(params, "name"),
		bserv::utils::security::encode_password(
			password.c_str()), false,
		get_or_empty(params, "major"),
		get_or_empty(params, "phone"),
		get_or_empty(params, "situation"), true);
	lginfo << r.query();
	auto bid = params["bid"].as_string();
	bserv::db_result b = tx.exec(
		"insert into ? "
		"(?, bid) values (?, ?)",
		bserv::db_name("building"),
		bserv::db_name("sid"),
		sid, bid);
	lginfo << b.query();
	tx.commit(); // you must manually commit changes
	return {
		{"success", true},
		{"message", "user registered"}
	};
}

boost::json::object inspection_apply(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	bserv::session_type& session = *session_ptr;
	if (params["iid"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`iid` is null"}
		};
	}
	if (params["inspect_date"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`inspect_date` is null"}
		};
	}
	auto sid = session["user"].as_object()["sid"].as_string();
	bserv::db_transaction tx{ conn };
	auto tmp1 = get_inspect_sid_date(tx, sid, params["inspect_date"].as_string());
	if (tmp1.has_value()) {
		return {
			{"success", false},
			{"message", "cannot inspect 2 times per day"}
		};
	}
	auto tmp2 = get_inspect_iid_date(tx, params["iid"].as_string(), params["inspect_date"].as_string());
	if (tmp2.has_value()) {
		return {
			{"success", false},
			{"message", "capacity full"}
		};
	}
	bserv::db_result r = tx.exec(
		"insert into ? "
		"(sid, iid, inspect_date, is_inspected, result) values "
		"(?, ?, ?, ?, ?)", bserv::db_name("inspection"),
		sid, get_or_empty(params, "iid"),
		get_or_empty(params, "inspect_date"),
		"NOT", "NULL");
	lginfo << r.query();
	tx.commit(); // you must manually commit changes
	return {
		{"success", true},
		{"message", "apply successfully"}
	};
}

boost::json::object user_login(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	if (params.count("sid") == 0) {
		return {
			{"success", false},
			{"message", "`sid` is required"}
		};
	}
	if (params.count("password") == 0) {
		return {
			{"success", false},
			{"message", "`password` is required"}
		};
	}
	auto sid = params["sid"].as_string();
	bserv::db_transaction tx{ conn };
	auto opt_user = get_user_sid(tx, sid);
	if (!opt_user.has_value()) {
		return {
			{"success", false},
			{"message", "invalid sid/password"}
		};
	}
	auto& user = opt_user.value();
	if (!user["is_active"].as_bool()) {
		return {
			{"success", false},
			{"message", "invalid sid/password"}
		};
	}
	auto password = params["password"].as_string();
	auto encoded_password = user["password"].as_string();
	if (!bserv::utils::security::check_password(
		password.c_str(), encoded_password.c_str())) {
		return {
			{"success", false},
			{"message", "invalid sid/password"}
		};
	}
	bserv::session_type& session = *session_ptr;
	session["user"] = user;
	return {
		{"success", true},
		{"message", "login successfully"}
	};
}

boost::json::object find_user(
	std::shared_ptr<bserv::db_connection> conn,
	const std::string& sid) {
	bserv::db_transaction tx{ conn };
	auto user = get_user_sid(tx, sid.c_str());
	if (!user.has_value()) {
		return {
			{"success", false},
			{"message", "requested user does not exist"}
		};
	}
	user.value().erase("sid");
	user.value().erase("password");
	return {
		{"success", true},
		{"user", user.value()}
	};
}

boost::json::object user_logout(
	std::shared_ptr<bserv::session_type> session_ptr) {
	bserv::session_type& session = *session_ptr;
	if (session.count("user")) {
		session.erase("user");
	}
	return {
		{"success", true},
		{"message", "logout successfully"}
	};
}

boost::json::object send_request(
	std::shared_ptr<bserv::session_type> session,
	std::shared_ptr<bserv::http_client> client_ptr,
	boost::json::object&& params) {
	auto obj = client_ptr->post_for_value(
		"localhost", "8080", "/echo", { {"request", params} }
	);
	if (session->count("cnt") == 0) {
		(*session)["cnt"] = 0;
	}
	(*session)["cnt"] = (*session)["cnt"].as_int64() + 1;
	return { {"response", obj}, {"cnt", (*session)["cnt"]} };
}

boost::json::object echo(
	boost::json::object&& params) {
	return { {"echo", params} };
}

// websocket
std::nullopt_t ws_echo(
	std::shared_ptr<bserv::session_type> session,
	std::shared_ptr<bserv::websocket_server> ws_server) {
	ws_server->write_json((*session)["cnt"]);
	while (true) {
		try {
			std::string data = ws_server->read();
			ws_server->write(data);
		}
		catch (bserv::websocket_closed&) {
			break;
		}
	}
	return std::nullopt;
}


std::nullopt_t serve_static_files(
	bserv::response_type& response,
	const std::string& path) {
	return serve(response, path);
}


std::nullopt_t index(
	const std::string& template_path,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	boost::json::object& context) {
	bserv::session_type& session = *session_ptr;
	if (session.contains("user")) {
		context["user"] = session["user"];
	}
	return render(response, template_path, context);
}

std::nullopt_t redirect_to_myself(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& my_user : my_users) {
		json_users.push_back(my_user);
	}
	context["my_users"] = json_users;
	if (session.contains("user"))
	{
		return index("normal_index.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t index_page(
	std::shared_ptr<bserv::session_type> session_ptr,
	std::shared_ptr<bserv::db_connection> conn,
	bserv::response_type& response) {
	boost::json::object context;
	bserv::session_type& session = *session_ptr;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return redirect_to_myself(conn, session_ptr, response, 1, std::move(context));
		}
		else return index("admin_index.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t form_login(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	lgdebug << params << std::endl;
	auto context = user_login(request, std::move(params), conn, session_ptr);
	lginfo << "login: " << context << std::endl;
	bserv::session_type& session = *session_ptr;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return redirect_to_myself(conn, session_ptr, response, 1, std::move(context));
		}
		else return index("admin_index.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t form_logout(
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response) {
	auto context = user_logout(session_ptr);
	lginfo << "logout: " << context << std::endl;
	return index("index.html", session_ptr, response, context);
}

std::nullopt_t redirect_to_users(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	lgdebug << "view users: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec("select count(*) from student, building "
		"where student.sid = building.sid and student.sid <> '123123123';");
	lginfo << db_res.query();
	std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total users: " << total_users << std::endl;
	int total_pages = (int)total_users / 10;
	if (total_users % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
		, (page_id - 1) * 10);
	lginfo << db_res.query();
	auto users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& user : users) {
		json_users.push_back(user);
	}
	
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_myusers;
	for (auto& my_user : my_users) {
		json_myusers.push_back(my_user);
	}
	context["my_users"] = json_myusers;
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["users"] = json_users;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_users.html", session_ptr, response, context);
		}
		else return index("admin_users.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t admin_redirect_to_users_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context,
	boost::json::object&& params) {
	bserv::session_type& session = *session_ptr;
	lgdebug << "view users: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	int total_pages;
	if (params.size() == 0) {
		session["user"].as_object()["phone"].as_string() = "1";
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = building.sid and student.sid <> '123123123';");
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	else {
		if (params["sid"].as_string().size() == 0 && params["bid"].as_string().size() == 0 && params["situation"].as_string().size() == 0) {
			session["user"].as_object()["phone"].as_string() = "1";
			db_res = tx.exec("select count(*) from student, building "
				"where student.sid = building.sid and student.sid <> '123123123';");
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["bid"].as_string().size() == 0 && params["situation"].as_string().size() == 0) {
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["phone"].as_string() = "2";
			db_res = tx.exec("select count(*) from student, building "
				"where student.sid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["sid"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where student.sid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["sid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() == 0 && params["bid"].as_string().size() != 0 && params["situation"].as_string().size() == 0) {
			session["user"].as_object()["bid"].as_string() = params["bid"].as_string();
			session["user"].as_object()["phone"].as_string() = "3";
			db_res = tx.exec("select count(*) from student, building "
				"where bid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["bid"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where bid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["bid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() == 0 && params["bid"].as_string().size() == 0 && params["situation"].as_string().size() != 0) {
			session["user"].as_object()["situation"].as_string() = params["situation"].as_string();
			session["user"].as_object()["phone"].as_string() = "4";
			db_res = tx.exec("select count(*) from student, building "
				"where situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["bid"].as_string().size() != 0 && params["situation"].as_string().size() == 0) {
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["bid"].as_string() = params["bid"].as_string();
			session["user"].as_object()["phone"].as_string() = "5";
			db_res = tx.exec("select count(*) from student, building "
				"where student.sid = ? and bid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where student.sid = ? and bid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["bid"].as_string().size() == 0 && params["situation"].as_string().size() != 0) {
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["situation"].as_string();
			session["user"].as_object()["phone"].as_string() = "6";
			db_res = tx.exec("select count(*) from student, building "
				"where student.sid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where student.sid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() == 0 && params["bid"].as_string().size() != 0 && params["situation"].as_string().size() != 0) {
			session["user"].as_object()["bid"].as_string() = params["bid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["situation"].as_string();
			session["user"].as_object()["phone"].as_string() = "7";
			db_res = tx.exec("select count(*) from student, building "
				"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["bid"].as_string().size() != 0 && params["situation"].as_string().size() != 0) {
			session["user"].as_object()["bid"].as_string() = params["bid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["situation"].as_string();
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["phone"].as_string() = "8";
			db_res = tx.exec("select count(*) from student, building "
				"where student.sid = ? and bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';",
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where student.sid = ? and bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;",
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
	}
	auto users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& user : users) {
		json_users.push_back(user);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["users"] = json_users;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_search.html", session_ptr, response, context);
		}
		else return index("admin_search.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t admin_redirect_to_users_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	lgdebug << "view users: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	db_res = tx.exec("select count(*) from student, building "
		"where student.sid = building.sid and student.sid <> '123123123';");
	lginfo << db_res.query();
	std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total users: " << total_users << std::endl;
	total_pages = (int)total_users / 10;
	if (total_users % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
		, (page_id - 1) * 10);
	lginfo << db_res.query();
	if (session["user"].as_object()["phone"].as_string() == "1") {
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = building.sid and student.sid <> '123123123';");
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "2") {
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["sid"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = ? and student.sid = building.sid and student.sid <> '123123123' 10 offset ?;", session["user"].as_object()["sid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "3") {
		db_res = tx.exec("select count(*) from student, building "
			"where bid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["bid"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where bid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["bid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "4") {
		db_res = tx.exec("select count(*) from student, building "
			"where situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "5") {
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = ? and bid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = ? and bid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "6") {
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "7") {
		db_res = tx.exec("select count(*) from student, building "
			"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "8") {
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = ? and bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = ? and bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	auto users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& user : users) {
		json_users.push_back(user);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["users"] = json_users;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_search.html", session_ptr, response, context);
		}
		else return index("admin_search.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t normal_redirect_to_users_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context,
	boost::json::object&& params) {
	bserv::session_type& session = *session_ptr;
	lgdebug << "view users: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_myusers;
	for (auto& my_user : my_users) {
		json_myusers.push_back(my_user);
	}
	context["my_users"] = json_myusers;
	if (params.size() == 0) {
		session["user"].as_object()["phone"].as_string() = "1";
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = building.sid and student.sid <> '123123123';");
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	else {
		if (params["bid"].as_string().size() == 0 && params["situation"].as_string().size() == 0) {
			session["user"].as_object()["phone"].as_string() = "1";
			db_res = tx.exec("select count(*) from student, building "
				"where student.sid = building.sid and student.sid <> '123123123';");
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["bid"].as_string().size() != 0 && params["situation"].as_string().size() == 0) {
			session["user"].as_object()["bid"].as_string() = params["bid"].as_string();
			session["user"].as_object()["phone"].as_string() = "2";
			db_res = tx.exec("select count(*) from student, building "
				"where bid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["bid"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where bid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["bid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["bid"].as_string().size() == 0 && params["situation"].as_string().size() != 0) {
			session["user"].as_object()["situation"].as_string() = params["situation"].as_string();
			session["user"].as_object()["phone"].as_string() = "3";
			db_res = tx.exec("select count(*) from student, building "
				"where situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["bid"].as_string().size() != 0 && params["situation"].as_string().size() != 0) {
			session["user"].as_object()["bid"].as_string() = params["bid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["situation"].as_string();
			session["user"].as_object()["phone"].as_string() = "4";
			db_res = tx.exec("select count(*) from student, building "
				"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';", 
				session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total users: " << total_users << std::endl;
			total_pages = (int)total_users / 10;
			if (total_users % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
				"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", 
				session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
	}
	auto users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& user : users) {
		json_users.push_back(user);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["users"] = json_users;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_search.html", session_ptr, response, context);
		}
		else return index("admin_search.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t normal_redirect_to_users_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	bserv::session_type& session = *session_ptr;
	lgdebug << "view users: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_myusers;
	for (auto& my_user : my_users) {
		json_myusers.push_back(my_user);
	}
	context["my_users"] = json_myusers;
	db_res = tx.exec("select count(*) from student, building "
		"where student.sid = building.sid and student.sid <> '123123123';");
	lginfo << db_res.query();
	std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total users: " << total_users << std::endl;
	total_pages = (int)total_users / 10;
	if (total_users % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
		, (page_id - 1) * 10);
	lginfo << db_res.query();
	if (session["user"].as_object()["phone"].as_string() == "1") {
		db_res = tx.exec("select count(*) from student, building "
			"where student.sid = building.sid and student.sid <> '123123123';");
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "2") {
		db_res = tx.exec("select count(*) from student, building "
			"where bid = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["bid"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where bid = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", session["user"].as_object()["bid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "3") {
		db_res = tx.exec("select count(*) from student, building "
			"where situation = ? and student.sid = building.sid and student.sid <> '123123123';", session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", 
			session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "4") {
		db_res = tx.exec("select count(*) from student, building "
			"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123';", 
			session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_users = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total users: " << total_users << std::endl;
		total_pages = (int)total_users / 10;
		if (total_users % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
			"where bid = ? and situation = ? and student.sid = building.sid and student.sid <> '123123123' order by student.sid limit 10 offset ?;", 
			session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	auto users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& user : users) {
		json_users.push_back(user);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["users"] = json_users;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_search.html", session_ptr, response, context);
		}
		else return index("admin_search.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t redirect_to_buildings(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	lgdebug << "view buildings: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec("select count(distinct bid) from building;");
	lginfo << db_res.query();
	std::size_t total_buildings = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total buildings: " << total_buildings << std::endl;
	int total_pages = (int)total_buildings / 10;
	if (total_buildings % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select bid, count(sid) "
		"from building where sid <> '123123123' group by bid order by bid asc "
		"limit 10 offset ?;", (page_id - 1) * 10);
	lginfo << db_res.query();
	auto buildings = building.convert_to_vector(db_res);
	boost::json::array json_buildings;
	for (auto& building : buildings) {
		json_buildings.push_back(building);
	}
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_myusers;
	for (auto& my_user : my_users) {
		json_myusers.push_back(my_user);
	}
	context["my_users"] = json_myusers;
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["buildings"] = json_buildings;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_buildings.html", session_ptr, response, context);
		}
		else return index("admin_buildings.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t redirect_to_inspections(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	lgdebug << "view inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec("select count(distinct iid) from inspection;");
	lginfo << db_res.query();
	std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total inspections: " << total_inspections << std::endl;
	int total_pages = (int)total_inspections / 10;
	if (total_inspections % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select inspect_date, iid, count(distinct sid) "
		"from inspection where sid <> '123123123'and is_inspected = 'NOT'" 
		"group by inspect_date, iid "
		"order by inspect_date, iid "
		"limit 10 offset ?;", (page_id - 1) * 10);
	lginfo << db_res.query();
	auto inspections = inspector.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& inspection : inspections) {
		json_inspections.push_back(inspection);
	}
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_myusers;
	for (auto& my_user : my_users) {
		json_myusers.push_back(my_user);
	}
	context["my_users"] = json_myusers;
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["inspections"] = json_inspections;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_inspections.html", session_ptr, response, context);
		}
		else return index("inspections.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t edit_redirect_to_inspections(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	lgdebug << "view inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec("select * from inspection;");
	lginfo << db_res.query();
	std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total inspections: " << total_inspections << std::endl;
	int total_pages = (int)total_inspections / 10;
	if (total_inspections % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select sid, iid, inspect_date, is_inspected, result from inspection "
		"order by inspect_date limit 10 offset ?;", (page_id - 1) * 10);
	lginfo << db_res.query();
	auto inspections = inspection.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& inspection : inspections) {
		json_inspections.push_back(inspection);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["inspections"] = json_inspections;
	bserv::session_type& session = *session_ptr;
	if (session.contains("user"))
	{
		return index("admin_inspections_edit.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t redirect_to_my_inspections(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user")) 
		return index("index.html", session_ptr, response, context);
	auto sid = session["user"].as_object()["sid"].as_string();
	lgdebug << "view my inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec("select count(*) from inspection "
		"where sid = ?;", sid);
	lginfo << db_res.query();
	std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total inspections: " << total_inspections << std::endl;
	int total_pages = (int)total_inspections / 10;
	if (total_inspections % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select sid, iid, inspect_date, is_inspected, result from inspection "
		"where sid = ? order by inspect_date limit 10 offset ?;", sid
		, (page_id - 1) * 10);
	lginfo << db_res.query();
	auto my_inspections = inspection.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& my_inspection : my_inspections) {
		json_inspections.push_back(my_inspection);
	}
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_myusers;
	for (auto& my_user : my_users) {
		json_myusers.push_back(my_user);
	}
	context["my_users"] = json_myusers;
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["my_inspections"] = json_inspections;
	if (session.contains("user"))
	{
		return index("normal_my_inspections.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t admin_redirect_to_inspections_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context,
	boost::json::object&& params) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	lgdebug << "view inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	if (params.size() == 0) {
		session["user"].as_object()["phone"].as_string() = "1";
		db_res = tx.exec("select count(*) from inspection;");
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	else {
		if (params["sid"].as_string().size() == 0 && params["iid"].as_string().size() == 0 && params["inspect_date"].as_string().size() == 0) {
			session["user"].as_object()["phone"].as_string() = "1";
			db_res = tx.exec("select count(*) from inspection;");
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["iid"].as_string().size() == 0 && params["inspect_date"].as_string().size() == 0) {
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["phone"].as_string() = "2";
			db_res = tx.exec("select count(*) from inspection "
				"where sid = ?;", session["user"].as_object()["sid"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where sid = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["sid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() == 0 && params["iid"].as_string().size() != 0 && params["inspect_date"].as_string().size() == 0) {
			session["user"].as_object()["bid"].as_string() = params["iid"].as_string();
			session["user"].as_object()["phone"].as_string() = "3";
			db_res = tx.exec("select count(*) from inspection "
				"where iid = ?;", session["user"].as_object()["bid"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where iid = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["bid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() == 0 && params["iid"].as_string().size() == 0 && params["inspect_date"].as_string().size() != 0) {
			session["user"].as_object()["situation"].as_string() = params["inspect_date"].as_string();
			session["user"].as_object()["phone"].as_string() = "4";
			db_res = tx.exec("select count(*) from inspection "
				"where inspect_date = ?;", session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where inspect_date = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["iid"].as_string().size() != 0 && params["inspect_date"].as_string().size() == 0) {
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["bid"].as_string() = params["iid"].as_string();
			session["user"].as_object()["phone"].as_string() = "5";
			db_res = tx.exec("select count(*) from inspection "
				"where sid = ? and iid = ?;", 
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where sid = ? and iid = ? order by inspect_date limit 10 offset ?;",
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["iid"].as_string().size() == 0 && params["inspect_date"].as_string().size() != 0) {
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["inspect_date"].as_string();
			session["user"].as_object()["phone"].as_string() = "6";
			db_res = tx.exec("select count(*) from inspection "
				"where sid = ? and inspect_date = ?;", 
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where sid = ? and inspect_date = ? order by inspect_date limit 10 offset ?;", 
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() == 0 && params["iid"].as_string().size() != 0 && params["inspect_date"].as_string().size() != 0) {
			session["user"].as_object()["bid"].as_string() = params["iid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["inspect_date"].as_string();
			session["user"].as_object()["phone"].as_string() = "7";
			db_res = tx.exec("select count(*) from inspection "
				"where iid = ? and inspect_date = ?;", session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where iid = ? and inspect_date = ? order by inspect_date limit 10 offset ?;", 
				session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["sid"].as_string().size() != 0 && params["iid"].as_string().size() != 0 && params["inspect_date"].as_string().size() != 0) {
			session["user"].as_object()["bid"].as_string() = params["iid"].as_string();
			session["user"].as_object()["situation"].as_string() = params["inspect_date"].as_string();
			session["user"].as_object()["sid"].as_string() = params["sid"].as_string();
			session["user"].as_object()["phone"].as_string() = "8";
			db_res = tx.exec("select count(*) from inspection "
				"where sid = ? and iid = ? and inspect_date = ?;",
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where sid = ? and iid = ? and inspect_date = ? order by inspect_date limit 10 offset ?;",
				session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
	}
	auto inspections = inspection.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& inspection : inspections) {
		json_inspections.push_back(inspection);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["inspections"] = json_inspections;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_inspections.html", session_ptr, response, context);
		}
		else return index("admin_inspections.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t admin_redirect_to_inspections_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	lgdebug << "view inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	db_res = tx.exec("select count(*) from inspection;");
	lginfo << db_res.query();
	std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total inspections: " << total_inspections << std::endl;
	total_pages = (int)total_inspections / 10;
	if (total_inspections % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
		, (page_id - 1) * 10);
	lginfo << db_res.query();
	if (session["user"].as_object()["phone"].as_string() == "1") {
		db_res = tx.exec("select count(*) from inspection;");
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "2") {
		db_res = tx.exec("select count(*) from inspection "
			"where sid = ?;", session["user"].as_object()["sid"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where sid = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["sid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "3") {
		db_res = tx.exec("select count(*) from inspection "
			"where iid = ?;", session["user"].as_object()["bid"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where iid = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["bid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "4") {
		db_res = tx.exec("select count(*) from inspection "
			"where inspect_date = ?;", session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where inspect_date = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "5") {
		db_res = tx.exec("select count(*) from inspection "
			"where sid = ? and iid = ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where sid = ? and iid = ? order by inspect_date limit 10 offset ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "6") {
		db_res = tx.exec("select count(*) from inspection "
			"where sid = ? and inspect_date = ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where sid = ? and inspect_date = ? order by inspect_date limit 10 offset ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "7") {
		db_res = tx.exec("select count(*) from inspection "
			"where iid = ? and inspect_date = ?;", session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where iid = ? and inspect_date = ? order by inspect_date limit 10 offset ?;",
			session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "8") {
		db_res = tx.exec("select count(*) from inspection "
			"where sid = ? and iid = ? and inspect_date = ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where sid = ? and iid = ? and inspect_date = ? order by inspect_date limit 10 offset ?;",
			session["user"].as_object()["sid"].as_string(), session["user"].as_object()["bid"].as_string(), session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	auto inspections = inspection.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& inspection : inspections) {
		json_inspections.push_back(inspection);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["inspections"] = json_inspections;
	if (session.contains("user"))
	{
		auto user = session["user"];
		auto is_superuser = user.as_object()["is_superuser"].as_bool();
		if (!is_superuser) {
			return index("normal_inspections.html", session_ptr, response, context);
		}
		else return index("admin_inspections.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t admin_edit_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context,
	boost::json::object&& params) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	lgdebug << "view inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	if (params.size() == 0) {
		session["user"].as_object()["phone"].as_string() = "1";
		db_res = tx.exec("select count(*) from inspection;");
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	else {
		if (params["is_inspected"].as_string().size() == 0) {
			session["user"].as_object()["phone"].as_string() = "1";
			db_res = tx.exec("select count(*) from inspection;");
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
		if (params["is_inspected"].as_string().size() != 0) {
			session["user"].as_object()["situation"].as_string() = params["is_inspected"].as_string();
			session["user"].as_object()["phone"].as_string() = "2";
			db_res = tx.exec("select count(*) from inspection "
				"where is_inspected = ?;", session["user"].as_object()["situation"].as_string());
			lginfo << db_res.query();
			std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
			lgdebug << "total inspections: " << total_inspections << std::endl;
			total_pages = (int)total_inspections / 10;
			if (total_inspections % 10 != 0) ++total_pages;
			lgdebug << "total pages: " << total_pages << std::endl;
			db_res = tx.exec(
				"select sid, iid, inspect_date, is_inspected, result from inspection "
				"where is_inspected = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
				, (page_id - 1) * 10);
			lginfo << db_res.query();
		}
	}
	auto inspections = inspection.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& inspection : inspections) {
		json_inspections.push_back(inspection);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["inspections"] = json_inspections;
	if (session.contains("user"))
	{
		return index("admin_inspections_edit.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t admin_edit_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	int page_id,
	boost::json::object&& context) {
	bserv::session_type& session = *session_ptr;
	if (!session.contains("user"))
		return index("index.html", session_ptr, response, context);
	lgdebug << "view inspections: " << page_id << std::endl;
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res;
	int total_pages;
	db_res = tx.exec("select count(*) from inspection;");
	lginfo << db_res.query();
	std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
	lgdebug << "total inspections: " << total_inspections << std::endl;
	total_pages = (int)total_inspections / 10;
	if (total_inspections % 10 != 0) ++total_pages;
	lgdebug << "total pages: " << total_pages << std::endl;
	db_res = tx.exec(
		"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
		, (page_id - 1) * 10);
	lginfo << db_res.query();
	if (session["user"].as_object()["phone"].as_string() == "1") {
		db_res = tx.exec("select count(*) from inspection;");
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection order by inspect_date limit 10 offset ?;"
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	if (session["user"].as_object()["phone"].as_string() == "2") {
		db_res = tx.exec("select count(*) from inspection "
			"where is_inspected = ?;", session["user"].as_object()["situation"].as_string());
		lginfo << db_res.query();
		std::size_t total_inspections = (*db_res.begin())[0].as<std::size_t>();
		lgdebug << "total inspections: " << total_inspections << std::endl;
		total_pages = (int)total_inspections / 10;
		if (total_inspections % 10 != 0) ++total_pages;
		lgdebug << "total pages: " << total_pages << std::endl;
		db_res = tx.exec(
			"select sid, iid, inspect_date, is_inspected, result from inspection "
			"where is_inspected = ? order by inspect_date limit 10 offset ?;", session["user"].as_object()["situation"].as_string()
			, (page_id - 1) * 10);
		lginfo << db_res.query();
	}
	auto inspections = inspection.convert_to_vector(db_res);
	boost::json::array json_inspections;
	for (auto& inspection : inspections) {
		json_inspections.push_back(inspection);
	}
	boost::json::object pagination;
	if (total_pages != 0) {
		pagination["total"] = total_pages;
		if (page_id > 1) {
			pagination["previous"] = page_id - 1;
		}
		if (page_id < total_pages) {
			pagination["next"] = page_id + 1;
		}
		int lower = page_id - 3;
		int upper = page_id + 3;
		if (page_id - 3 > 2) {
			pagination["left_ellipsis"] = true;
		}
		else {
			lower = 1;
		}
		if (page_id + 3 < total_pages - 1) {
			pagination["right_ellipsis"] = true;
		}
		else {
			upper = total_pages;
		}
		pagination["current"] = page_id;
		boost::json::array pages_left;
		for (int i = lower; i < page_id; ++i) {
			pages_left.push_back(i);
		}
		pagination["pages_left"] = pages_left;
		boost::json::array pages_right;
		for (int i = page_id + 1; i <= upper; ++i) {
			pages_right.push_back(i);
		}
		pagination["pages_right"] = pages_right;
		context["pagination"] = pagination;
	}
	context["inspections"] = json_inspections;
	if (session.contains("user"))
	{
		return index("admin_inspections_edit.html", session_ptr, response, context);
	}
	else return index("index.html", session_ptr, response, context);
}

std::nullopt_t view_buildings(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return redirect_to_buildings(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t view_my_inspections(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return redirect_to_my_inspections(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t view_inspections(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return redirect_to_inspections(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t view_users(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return redirect_to_users(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t admin_view_users_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num,
	boost::json::object&& params) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return admin_redirect_to_users_restrict(conn, session_ptr, response, page_id, std::move(context), std::move(params));
}
std::nullopt_t admin_view_users_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return admin_redirect_to_users_restrict_save(conn, session_ptr, response, page_id, std::move(context));
}
std::nullopt_t normal_view_users_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num,
	boost::json::object&& params) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return normal_redirect_to_users_restrict(conn, session_ptr, response, page_id, std::move(context), std::move(params));
}
std::nullopt_t normal_view_users_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return normal_redirect_to_users_restrict_save(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t admin_view_inspections_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num,
	boost::json::object&& params) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return admin_redirect_to_inspections_restrict(conn, session_ptr, response, page_id, std::move(context), std::move(params));
}
std::nullopt_t admin_view_inspections_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return admin_redirect_to_inspections_restrict_save(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t admin_view_inspections_edit_restrict(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num,
	boost::json::object&& params) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return admin_edit_restrict(conn, session_ptr, response, page_id, std::move(context), std::move(params));
}
std::nullopt_t admin_view_inspections_edit_restrict_save(
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr,
	bserv::response_type& response,
	const std::string& page_num) {
	int page_id = std::stoi(page_num);
	boost::json::object context;
	return admin_edit_restrict_save(conn, session_ptr, response, page_id, std::move(context));
}

std::nullopt_t form_add_user(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = user_register(request, std::move(params), conn);
	return redirect_to_users(conn, session_ptr, response, 1, std::move(context));
}

std::nullopt_t form_apply(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = inspection_apply(request, std::move(params), conn, session_ptr);
	return redirect_to_inspections(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object user_update(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	if (params.count("sid") == 0) {
		return {
			{"success", false},
			{"message", "`sid` is required"}
		};
	}
	if (params["sid"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`sid` is null"}
		};
	}
	auto sid = params["sid"].as_string();
	bserv::db_transaction tx{ conn };
	auto opt_user = get_user_sid(tx, sid);
	if (!opt_user.has_value())
		return {
		{"success", false},
		{"message", "user not found"} };
	if (params["name"].as_string().size() != 0) {
		bserv::db_result r = tx.exec(
			"update ? "
			"set name = ? where sid = ?", bserv::db_name("student"),
			get_or_empty(params, "name"), sid);
		lginfo << r.query();
	}
	if (params["major"].as_string().size() != 0) {
		bserv::db_result r = tx.exec(
			"update ? "
			"set major = ? where sid = ?", bserv::db_name("student"),
			get_or_empty(params, "major"), sid);
		lginfo << r.query();
	}
	if (params["phone"].as_string().size() != 0) {
		bserv::db_result r = tx.exec(
			"update ? "
			"set phone = ? where sid = ?", bserv::db_name("student"),
			get_or_empty(params, "phone"), sid);
		lginfo << r.query();
	}
	if (params["situation"].as_string().size() != 0) {
		bserv::db_result r = tx.exec(
			"update ? "
			"set situation = ? where sid = ?", bserv::db_name("student"),
			get_or_empty(params, "situation"), sid);
		lginfo << r.query();
	}
	if (params["bid"].as_string().size() != 0) {
		bserv::db_result r = tx.exec(
			"update ? "
			"set bid = ? where sid = ?", bserv::db_name("building"),
			get_or_empty(params, "bid"), sid);
		lginfo << r.query();
	}
	tx.commit(); // you must manually commit changes
	return {
		{"success", true},
		{"message", "user updated"}
	};
}

std::nullopt_t form_update_user(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = user_update(request, std::move(params), conn);
	return redirect_to_users(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object inspection_update(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	if (params.count("sid") == 0) {
		return {
			{"success", false},
			{"message", "`sid` is required"}
		};
	}
	if (params["sid"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`sid` is null"}
		};
	}
	if (params["inspect_date"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`inspect_date` is null"}
		};
	}
	bserv::db_transaction tx{ conn };
	auto sid = params["sid"].as_string();
	auto inspect_date = params["inspect_date"].as_string();
	if (params["is_inspected"].as_string().size() != 0) {
		auto is_inspected = params["is_inspected"].as_string();
		bserv::db_result r = tx.exec(
			"update ? "
			"set is_inspected = ? where sid = ? and inspect_date = ?", bserv::db_name("inspection"),
			is_inspected, sid, inspect_date);
		lginfo << r.query();
	}
	if (params["result"].as_string().size() != 0) {
		auto result = params["result"].as_string();
		bserv::db_result r = tx.exec(
			"update ? "
			"set result = ? where sid = ? and inspect_date = ?", bserv::db_name("inspection"),
			result, sid, inspect_date);
		lginfo << r.query();
		bserv::db_result a = tx.exec(
			"update ? "
			"set situation = ? where sid = ?", bserv::db_name("student"),
			result, sid);
		lginfo << a.query();
	}
	tx.commit(); // you must manually commit changes
	return {
		{"success", true},
		{"message", "situation updated"}
	};
}

std::nullopt_t form_update_inspection(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = inspection_update(request, std::move(params), conn);
	return edit_redirect_to_inspections(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object user_update_my_inspection(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	bserv::session_type& session = *session_ptr;
	auto tmp = session["user"].as_object()["sid"];
	auto sid = tmp.as_string();
	if (params.count("inspect_date") == 0) {
		return {
			{"success", false},
			{"message", "`inspect_date` is required"}
		};
	}
	if (params["inspect_date"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`inspect_date` is null"}
		};
	}
	bserv::db_transaction tx{ conn };
	auto inspect_date = params["inspect_date"].as_string();
	bserv::db_result r = tx.exec(
		"update ? "
		"set is_inspected = 'YES' where sid = ? and inspect_date = ?", bserv::db_name("inspection"),
		sid, inspect_date);
	lginfo << r.query();
	tx.commit(); // you must manually commit changes
	return {
		{"success", true},
		{"message", "inspection checked"}
	};
}

std::nullopt_t form_update_my_inspection(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = user_update_my_inspection(request, std::move(params), conn, session_ptr);
	return redirect_to_my_inspections(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object user_update_self(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	bserv::session_type& session = *session_ptr;
	auto sid = session["user"].as_object()["sid"].as_string();
	if (params.count("password") == 0) {
		return {
			{"success", false},
			{"message", "`password` is required"}
		};
	}
	if (params["password"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`password` is null"}
		};
	}
	bserv::db_transaction tx{ conn };
	bserv::db_result db_res = tx.exec(
		"select student.sid, name, password, is_superuser, major, phone, bid, situation, is_active from student, building "
		"where student.sid = ? and student.sid = building.sid;", sid);
	lginfo << db_res.query();
	auto my_users = user.convert_to_vector(db_res);
	boost::json::array json_users;
	for (auto& my_user : my_users) {
		json_users.push_back(my_user);
	}
	auto password = params["password"].as_string();
	auto encoded_password = json_users.begin()->as_object()["password"].as_string();
	if (bserv::utils::security::check_password(
		password.c_str(), encoded_password.c_str())) {
		return {
			{"success", false},
			{"message", "please enter a new password"}
		};
	}
	bserv::db_result r = tx.exec(
		"update ? "
		"set password = ? where sid = ?", bserv::db_name("student"),
		bserv::utils::security::encode_password(
			password.c_str()), sid);
	lginfo << r.query();
	tx.commit(); // you must manually commit changes
	return {
		{"success", true},
		{"message", "password updated"}
	};
}

std::nullopt_t form_update_self(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = user_update_self(request, std::move(params), conn, session_ptr);
	return redirect_to_myself(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object delete_user(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	if (params.count("sid") == 0) {
		return {
			{"success", false},
			{"message", "`sid` is required"}
		};
	}
	auto sid = params["sid"].as_string();
	bserv::db_transaction tx{ conn };
	auto opt_user = get_user_sid(tx, sid);
	if(!opt_user.has_value())
		return {
		{"success", false},
		{"message", "user not found"}};
	bserv::db_result a = tx.exec(
		"delete from ? "
		"where sid = ?;"
		, bserv::db_name("building"), sid);
	lginfo << a.query();
	bserv::db_result b = tx.exec(
		"delete from ? "
		"where sid = ?;"
		, bserv::db_name("inspection"), sid);
	lginfo << b.query();
	bserv::db_result r = tx.exec(
		"delete from ? "
		"where sid = ?;"
		,bserv::db_name("student"), sid);
	lginfo << r.query();
	tx.commit();
	return {
		{"success", true},
		{"message", "user deleted"}
	};
}

std::nullopt_t form_delete_user(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = delete_user(request, std::move(params), conn);
	return redirect_to_users(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object delete_my_inspection(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	bserv::session_type& session = *session_ptr;
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	auto sid = session["user"].as_object()["sid"].as_string();
	if (params["inspect_date"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`inspect_date` is required"}
		};
	}
	auto inspect_date = params["inspect_date"].as_string();
	auto is_inspected = params["is_inspected"].as_string();
	bserv::db_transaction tx{ conn };
	bserv::db_result b = tx.exec(
		"delete from ? "
		"where sid = ? and inspect_date = ?;"
		, bserv::db_name("inspection"), sid, inspect_date);
	lginfo << b.query();
	tx.commit();
	if (is_inspected == "YES") {
		return {
			{"success", true},
			{"message", "inspection deleted"}
		};
	}
	if (is_inspected == "NOT") {
		return {
			{"success", true},
			{"message", "inspection canceled"}
		};
	}
}

std::nullopt_t form_delete_my_inspection(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = delete_my_inspection(request, std::move(params), conn, session_ptr);
	return redirect_to_my_inspections(conn, session_ptr, response, 1, std::move(context));
}

boost::json::object delete_inspection(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn) {
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	if (params["sid"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`sid` is required"}
		};
	}
	if (params["inspect_date"].as_string().size() == 0) {
		return {
			{"success", false},
			{"message", "`inspect_date` is required"}
		};
	}
	auto sid = params["sid"].as_string();
	auto inspect_date = params["inspect_date"].as_string();
	bserv::db_transaction tx{ conn };
	bserv::db_result a = tx.exec(
		"delete from ? "
		"where sid = ? and inspect_date = ?;"
		, bserv::db_name("inspection"), sid, inspect_date);
	lginfo << a.query();
	tx.commit();
	return {
		{"success", true},
		{"message", "inspection deleted"}
	};
}

std::nullopt_t form_delete_inspection(
	bserv::request_type& request,
	bserv::response_type& response,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn,
	std::shared_ptr<bserv::session_type> session_ptr) {
	boost::json::object context = delete_inspection(request, std::move(params), conn);
	return edit_redirect_to_inspections(conn, session_ptr, response, 1, std::move(context));
}