/*
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2,
 * as published by the Free Software Foundation.
 *
 * In addition to the permissions in the GNU General Public License,
 * the authors give you unlimited permission to link the compiled
 * version of this file into combinations with other programs,
 * and to distribute those combinations without any restriction
 * coming from the use of this file.  (The General Public License
 * restrictions do apply in other respects; for example, they cover
 * modification of the file, and distribution when not linked into
 * a combined executable.)
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <assert.h>
#include <string.h>
#include <git2.h>
#include <git2/sys/odb_backend.h>
#include <git2/sys/refdb_backend.h>
#include <git2/sys/refs.h>
#include <bson.h>
#include <mongoc.h>

typedef struct {
	git_odb_backend parent;

	char *prefix;
	char *repo_path;
	mongoc_client_t *client;
} mongoc_odb_backend;

typedef struct {
	git_refdb_backend parent;

	char *prefix;
	char *repo_path;
	mongoc_client_t *client;
} mongoc_refdb_backend;

typedef struct {
	git_reference_iterator parent;

	size_t current;
	// TODO: redisReply *keys;

	mongoc_refdb_backend *backend;
} mongoc_refdb_iterator;

// TODO: static redisContext *sharedConnnection = NULL;

/* odb methods */

int mongoc_odb_backend__read_header(size_t *len_p, git_otype *type_p, git_odb_backend *_backend, const git_oid *oid)
{
	mongoc_odb_backend *backend;
	int error;
	
