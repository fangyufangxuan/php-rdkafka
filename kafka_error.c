/*
  +----------------------------------------------------------------------+
  | php-rdkafka                                                          |
  +----------------------------------------------------------------------+
  | Copyright (c) 2016 Arnaud Le Blanc                                   |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Arnaud Le Blanc <arnaud.lb@gmail.com>                        |
  +----------------------------------------------------------------------+
*/

#include "php.h"
#include "php_rdkafka.h"
#include "php_rdkafka_priv.h"
#include "librdkafka/rdkafka.h"
#include "Zend/zend_exceptions.h"
#include "kafka_error.h"
#include "zeval.h"

typedef struct _object_intern {
#if PHP_MAJOR_VERSION < 7
    zend_object                 std;
#endif
    rd_kafka_error_t *error;
#if PHP_MAJOR_VERSION >= 7
    zend_object                 std;
#endif
} object_intern;

zend_class_entry * ce_kafka_error;

static void kafka_conf_free(zend_object *object TSRMLS_DC) /* {{{ */
{
    object_intern *intern = get_custom_object(object_intern, object);

    rd_kafka_error_destroy(intern->error);

    zend_object_std_dtor(&intern->std TSRMLS_CC);

    free_custom_object(intern);
}

static zend_object_value kafka_error_new(zval *return_value, const rd_kafka_error_t *error TSRMLS_DC) /* {{{ */
{
    kafka_error_object *oerr;

    object_intern = alloc_object(oerr, ce_kafka_error);
    zend_object_std_init(&object_intern->std, ce_kafka_error TSRMLS_CC);
    object_properties_init(&object_intern->std, ce_kafka_error);

    object_intern->error = error;

    STORE_OBJECT(retval, object_intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, kafka_error_free, NULL);
    SET_OBJECT_HANDLERS(retval, &handlers);

    return retval;
}

kafka_error_object * get_kafka_error_object(zval *zerr TSRMLS_DC)
{
    kafka_error_object *oerr = get_custom_object_zval(kafka_error_object, zerr);

    return oerr;
}

/* {{{ private constructor */
PHP_METHOD(RdKafka__KafkaError, __construct)
{
    zend_throw_exception(NULL, "Private constructor", 0 TSRMLS_CC);
    return;
}
/* }}} */

/* {{{ proto void RdKafka\KafkaError::getCode()
    Get code of error */

ZEND_BEGIN_ARG_INFO_EX(arginfo_kafka_error_get_code, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(RdKafka__KafkaError, getCode)
{
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    intern = get_object(getThis() TSRMLS_CC);

    RETURN_LONG(rd_kafka_error_code(intern->error));
}
/* }}} */

/* {{{ proto void RdKafka\KafkaError::getName()
    Get name of error */

ZEND_BEGIN_ARG_INFO_EX(arginfo_kafka_error_get_name, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(RdKafka__KafkaError, getName)
{
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    intern = get_object(getThis() TSRMLS_CC);

    RETURN_STRING(rd_kafka_error_name(intern->error));
}
/* }}} */

/* {{{ proto void RdKafka\KafkaError::getMessage()
    Get message of error */

ZEND_BEGIN_ARG_INFO_EX(arginfo_kafka_error_get_message, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(RdKafka__KafkaError, getMessage)
{
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    intern = get_object(getThis() TSRMLS_CC);

    RETURN_STRING(rd_kafka_error_string(intern->error));
}
/* }}} */

/* {{{ proto void RdKafka\KafkaError::isFatal()
    Return true if error is fatal */

ZEND_BEGIN_ARG_INFO_EX(arginfo_kafka_error_is_fatal, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(RdKafka__KafkaError, isFatal)
{
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    intern = get_object(getThis() TSRMLS_CC);

    RETURN(1 == rd_kafka_error_is_fatal(intern->error));
}
/* }}} */

/* {{{ proto void RdKafka\KafkaError::isRetriable()
    Return true if error is fatal */

ZEND_BEGIN_ARG_INFO_EX(arginfo_kafka_error_is_retriable, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(RdKafka__KafkaError, isRetriable)
{
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    intern = get_object(getThis() TSRMLS_CC);

    RETURN(1 == rd_kafka_error_is_retriable(intern->error));
}
/* }}} */

/* {{{ proto void RdKafka\KafkaError::transactionRequiresAbort()
    Return true if error is fatal */

ZEND_BEGIN_ARG_INFO_EX(arginfo_kafka_error_transaction_requires_abort, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(RdKafka__KafkaError, transactionRequiresAbort)
{
    object_intern *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        return;
    }

    intern = get_object(getThis() TSRMLS_CC);

    RETURN_BOOL(1 == rd_kafka_error_txn_requires_abort(intern->error));
}
/* }}} */

static const zend_function_entry kafka_error_fe[] = { /* {{{ */
    PHP_ME(RdKafka__KafkaError, getCode, arginfo_kafka_error_get_code, ZEND_ACC_PUBLIC)
    PHP_ME(RdKafka__KafkaError, getName, arginfo_kafka_error_get_ name, ZEND_ACC_PUBLIC)
    PHP_ME(RdKafka__KafkaError, getMessage, arginfo_kafka_error_get_messge, ZEND_ACC_PUBLIC)
    PHP_ME(RdKafka__KafkaError, isFatal, arginfo_kafka_error_is_fatal, ZEND_ACC_PUBLIC)
    PHP_ME(RdKafka__KafkaError, isRetriable, arginfo_kafka_error_is_retriable, ZEND_ACC_PUBLIC)
    PHP_ME(RdKafka__KafkaError, transactionRequiresAbort, arginfo_kafka_error_transaction_requires_abort, ZEND_ACC_PUBLIC)
    PHP_FE_END
}; /* }}} */

void kafka_error_minit(TSRMLS_D) /* {{{ */
{
    zend_class_entry ce;

    handlers = kafka_default_object_handlers;
    set_object_handler_free_obj(&handlers, kafka_error_free);
    set_object_handler_offset(&handlers, XtOffsetOf(kafka_error_object, std));

    INIT_NS_CLASS_ENTRY(ce, "RdKafka", "KafkaError", kafka_error_fe);
    ce_kafka_error = zend_register_internal_class(&ce TSRMLS_CC);
} /* }}} */
