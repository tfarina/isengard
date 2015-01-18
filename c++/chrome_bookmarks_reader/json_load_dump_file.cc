/*
 * Copyright (c) 2009 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdio.h>

#include <jansson.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s in-file out-file\n", argv[0]);
    return 2;
  }

  json_error_t error;
  json_t* json = json_load_file(argv[1], 0, &error);
  if (!json) {
    fprintf(stderr, "%d\n%s\n", error.line, error.text);
    return 1;
  }

  json_t* bookmarks_object = json_object_get(json, "roots");
  if (!bookmarks_object) {
    fprintf(stderr, "%s\n", "no bookmarks object");
    return 1;
  }

  printf("%s\n", json_dumps(bookmarks_object, 0));

  // bookmarks_bar is the first root.
  void* iter = json_object_iter(bookmarks_object);
  const char* iter_key = json_object_iter_key(iter);
  printf("iter key: %s\n", iter_key);

  json_t* iter_value = json_object_iter_value(iter);
  printf("iter value: %s\n", json_dumps(iter_value, 0));

  json_t* name = json_object_get(iter_value, "name");
  const char* name_value = json_string_value(name);
  printf("name: %s\n", name_value);

  // now here I can get the type!
  json_t* type = json_object_get(iter_value, "type");
  const char* type_value = json_string_value(type);
  printf("type: %s\n", type_value);

  //json_t* children = json_object_get(iter_value, "children");

  // now you can get any element in the |children| array:
  //json_t* element = json_array_get(children, 0);

  printf("advance\n");

  // synced is the third root.
  iter = json_object_iter_next(bookmarks_object, iter);
  iter_key = json_object_iter_key(iter);
  printf("iter key: %s\n", iter_key);

  //json_dump_file(json, argv[2], 0);
  json_decref(json);

  return 0;
}
