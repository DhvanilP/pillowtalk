#include <iostream>
#include <string>
#include <vector>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "pillowtalk.h"

using namespace std;
using namespace boost::unit_test;

struct InitFixture {
  InitFixture() {
    pt_init();
    pt_response_t* res;

    res = pt_delete("http://admin:mini1234@localhost:5984/pt_test");
    if (res->response_code == 500) {
      pt_free_response(res);
      cout << "Please ensure that couchdb is running on localhost" << endl;
      exit(-1);
    }
    pt_free_response(res);

    res = pt_put_raw("http://admin:mini1234@localhost:5984/pt_test", NULL, 0);
    pt_free_response(res);

    const char* basic = "{}";
    res = pt_put_raw("http://admin:mini1234@localhost:5984/pt_test/basic", basic, strlen(basic));
    pt_free_response(res);

    const char* array = "{\"a\":[1,2,3]}";
    res = pt_put_raw("http://admin:mini1234@localhost:5984/pt_test/array", array, strlen(array));
    pt_free_response(res);
  }


  ~InitFixture() {
    pt_cleanup();
  }
};

inline void assert_valid_http_code(int code)
{
    BOOST_REQUIRE(code >= 200 && code < 300);
}

//BOOST_FIXTURE_TEST_SUITE(s, InitFixture);
BOOST_GLOBAL_FIXTURE(InitFixture);

BOOST_AUTO_TEST_CASE( test_basic )
{
  pt_response_t* res = pt_get("http://admin:mini1234@localhost:5984/pt_test/basic");
  BOOST_REQUIRE(res);
  BOOST_REQUIRE(res->root);
  BOOST_REQUIRE(res->root->type == PT_MAP);
  pt_node_t* id = pt_map_get(res->root,"_id");
  BOOST_REQUIRE(id);
  BOOST_REQUIRE(id->type == PT_STRING);
  const char* val = pt_string_get(id);
  BOOST_REQUIRE_EQUAL(val,"basic");

  pt_free_response(res);
}

BOOST_AUTO_TEST_CASE( test_array )
{
  pt_response_t* res = pt_get("http://admin:mini1234@localhost:5984/pt_test/array");
  BOOST_REQUIRE(res->root);
  BOOST_REQUIRE(res->root->type == PT_MAP);

  pt_node_t* array = pt_map_get(res->root,"a");
  BOOST_REQUIRE(array);
  BOOST_REQUIRE(array->type == PT_ARRAY);
  unsigned int len = pt_array_len(array);
  BOOST_REQUIRE_EQUAL(len,3);
  pt_free_response(res);
}

BOOST_AUTO_TEST_CASE( test_updates_to_document )
{
  pt_node_t* new_doc = pt_map_new();
  pt_node_t* id = pt_string_new("mynewdoc");
  pt_map_set(new_doc, "_id", id);

  pt_node_t* name = pt_string_new("jubos");
  pt_map_set(new_doc, "name", name);
  pt_map_set(new_doc, "updates", pt_array_new());

  pt_response_t* res = pt_put("http://admin:mini1234@localhost:5984/pt_test/mynewdoc", new_doc);
  assert_valid_http_code(res->response_code);
  BOOST_REQUIRE(pt_map_get(res->root, "rev") != NULL);
  pt_free_response(res);
  pt_free_node(new_doc);

  for(int i=0; i < 5; i++) {
    res = pt_get("http://admin:mini1234@localhost:5984/pt_test/mynewdoc");
    BOOST_REQUIRE(res->response_code == 200);
    pt_array_push_back(pt_map_get(res->root, "updates"), pt_integer_new(i));
    pt_response_t* put_res = pt_put("http://admin:mini1234@localhost:5984/pt_test/mynewdoc", res->root);
    BOOST_REQUIRE(put_res->response_code >= 200 && put_res->response_code < 300);
    pt_free_response(res);
    pt_free_response(put_res);
  }
}

BOOST_AUTO_TEST_CASE( test_create_document_via_post )
{
  pt_node_t* new_doc = pt_map_new();
  pt_node_t* id = pt_string_new("mynewdoc-post");
  pt_map_set(new_doc, "_id", id);

  pt_node_t* name = pt_string_new("jubos");
  pt_map_set(new_doc, "name", name);
  pt_map_set(new_doc, "updates", pt_array_new());

  pt_response_t* res = pt_post("http://admin:mini1234@localhost:5984/pt_test", new_doc);

  assert_valid_http_code(res->response_code);
  BOOST_REQUIRE(pt_map_get(res->root, "rev") != NULL);

  /* cleanup */
  pt_free_response(res);
  pt_free_node(new_doc);
}

BOOST_AUTO_TEST_CASE( test_create_document_via_post_raw )
{
  const char *doc = "{"
    "\"_id\": \"mynewdoc-post-raw\","
    "\"name\": \"jubos\""
    "}";
  pt_response_t* res = pt_post_raw(
    "http://admin:mini1234@localhost:5984/pt_test", doc, strlen(doc));

  assert_valid_http_code(res->response_code);
  BOOST_REQUIRE(pt_map_get(res->root, "rev") != NULL);

  /* cleanup */
  pt_free_response(res);
}

// Here we make a new set of json and make sure we get what we expect
BOOST_AUTO_TEST_CASE( test_mutable_json )
{
  pt_node_t* map = pt_map_new();
  BOOST_REQUIRE(map);
  BOOST_REQUIRE(map->type == PT_MAP);

  pt_node_t* null = pt_null_new();
  BOOST_REQUIRE(null);
  BOOST_REQUIRE(null->type == PT_NULL);

  pt_node_t* boolean = pt_bool_new(1);
  BOOST_REQUIRE(boolean);
  BOOST_REQUIRE(boolean->type == PT_BOOLEAN);

  pt_node_t* integer = pt_integer_new(100);
  BOOST_REQUIRE(integer);
  BOOST_REQUIRE(integer->type == PT_INTEGER);

  pt_node_t* dbl = pt_double_new(9.99);
  BOOST_REQUIRE(dbl);
  BOOST_REQUIRE(dbl->type == PT_DOUBLE);

  pt_node_t* world = pt_string_new("string");
  BOOST_REQUIRE(world);
  BOOST_REQUIRE(world->type == PT_STRING);

  pt_node_t* ary = pt_array_new();
  BOOST_REQUIRE(ary);
  BOOST_REQUIRE(ary->type == PT_ARRAY);

  pt_node_t* int1 = pt_integer_new(1);
  pt_node_t* int2 = pt_integer_new(2);
  pt_node_t* int3 = pt_integer_new(3);
  pt_array_push_back(ary,int1);
  pt_array_push_back(ary,int2);
  pt_array_push_back(ary,int3);

  BOOST_REQUIRE_EQUAL(pt_array_len(ary),3);

  pt_array_remove(ary,int2);

  BOOST_REQUIRE_EQUAL(pt_array_len(ary),2);


  // Now set these things into the map

  pt_map_set(map,"null",null);
  pt_map_set(map,"boolean",boolean);
  pt_map_set(map,"integer",integer);
  pt_map_set(map,"double",dbl);
  pt_map_set(map,"string",world);
  pt_map_set(map,"array",ary);

  pt_node_t* value = pt_map_get(map,"null");
  BOOST_REQUIRE(value->type == PT_NULL);

  value = pt_map_get(map,"boolean");
  BOOST_REQUIRE(pt_boolean_get(value) == 1);

  value = pt_map_get(map,"integer");
  BOOST_REQUIRE(pt_integer_get(value) == 100);

  value = pt_map_get(map,"double");
  BOOST_REQUIRE(pt_double_get(value) == 9.99);

  value = pt_map_get(map,"string");
  BOOST_REQUIRE(value);
  BOOST_REQUIRE_EQUAL("string",pt_string_get(value));

  value = pt_map_get(map,"array");
  BOOST_REQUIRE(value);


  pt_map_unset(map,"string");
  BOOST_REQUIRE(!pt_map_get(map,"string"));

  pt_free_node(map);
}

//BOOST_AUTO_TEST_SUITE_END()
