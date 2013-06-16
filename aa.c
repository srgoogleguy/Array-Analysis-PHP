#include <aa.h>
#include <main/SAPI.h>
#include <ext/standard/info.h>

int get_array_size(zval *array, zend_bool d)
{
    zval **entry, *v;
    uint str_key_len;
    int i, t, c = 0;
    Bucket * p, * l;
    long long arSize = 0; /* Stores the total size of memory consumed in bytes */
    

    /* First we get the size of the zval struct */
    arSize += sizeof(*array) + sizeof(array->value);
    if (d) {
        printf("ZVAL = %ld\n", sizeof(*array) + sizeof(array->value));
    }
    
    /* Second we get the size of the HashTable struct */
    arSize += sizeof(*array->value.ht);
    if (d) {
        printf("HashTable = %ld\n", sizeof(*array->value.ht));
    }
    
    /* If the array is empty we can safely exit here */
    t = array->value.ht->nNumOfElements;
    if (!t) {
        return arSize;
    }
    
    /* Third we get the size of each bucket struct in the HashTable array */
    for (i = 0; i < array->value.ht->nTableSize; i++) {
        /* If the array has no bucket continue */
        if (!array->value.ht->arBuckets[i]) {
            continue;
        }
        /* Otherwise we need to check both the bucket as well as any buckets it links to next */
        arSize += sizeof(*array->value.ht->arBuckets[i]);        
        arSize += array->value.ht->arBuckets[i]->nKeyLength;
        if (d) {
            printf("Bucket[%d] = %ld\n", i, sizeof(*array->value.ht->arBuckets[i]) + array->value.ht->arBuckets[i]->nKeyLength);
        }
        if (zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&entry, &array->value.ht->arBuckets[i]) == SUCCESS) {
            v = *entry;
            switch(v->type) {
                case IS_STRING:
                    arSize += Z_STRLEN_P(v) + sizeof(v->value);
                    if (d) {
                        printf("Bucket[%d].data.string = %ld\n", i, Z_STRLEN_P(v) + sizeof(v->value));
                    }
                    break;
                case IS_ARRAY:
                    arSize += sizeof(*v);
                    arSize += sizeof(*v->value.ht);
                    if (d) {
                        printf("Bucket[%d].data.ht = %ld\n", i, sizeof(*v) + sizeof(*v->value.ht));
                    }
                    arSize += get_array_size(v, d); /* Recursively walk the array */
                    break;
                default:
                    arSize += sizeof(*v) + sizeof(v->value);
                    if (d) {
                        printf("Bucket[%d].data.val = %ld\n", i, sizeof(*v) + sizeof(v->value));
                    }
                    break;                
            }
        }
        p = array->value.ht->arBuckets[i]->pNext;
        while (p != NULL) {
            if (d) {
                printf("** Collision Detected **\n");
            }
            ++c; /* Increment the collision counter */
            /* Get the size of the Bucket struct */
            arSize += sizeof(*p);
            arSize += p->nKeyLength;
            if (d) {
                printf("Bucket[%d] = %ld\n", i, sizeof(*p) + p->nKeyLength);
            }
            /* If the zval is a string or array we also need to get the size of the data they store */
            if (zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&entry, &p) == SUCCESS) {
                v = *entry;
                switch(v->type) {
                    case IS_STRING:
                        arSize += Z_STRLEN_P(v);
                        arSize += Z_STRLEN_P(v) + sizeof(v->value);
                        if (d) {
                            printf("Bucket[%d].data.string = %ld\n", i, Z_STRLEN_P(v) + sizeof(v->value));
                        }
                        break;
                    case IS_ARRAY:
                        arSize += sizeof(*v);
                        arSize += sizeof(*v->value.ht);
                        if (d) {
                            printf("Bucket[%d].data.ht = %ld\n", i, sizeof(*v) + sizeof(*v->value.ht));
                        }
                        arSize += get_array_size(v, d); /* Recursively walk the array */
                        break;
                    default:
                        arSize += sizeof(*v) + sizeof(v->value);
                        if (d) {
                            printf("Bucket[%d].data.val = %ld\n", i, sizeof(*v) + sizeof(v->value));
                        }
                        break;                
                }
            }
            p = p->pNext;
        }
    }
    
    if (d) {
        printf("%d total collisions out of %d elements\n", c, t);
    }
    
    return arSize;    
    
}

PHP_FUNCTION(array_size)
{
    zval *array;
    zend_bool d;
    long long arSize = 0; /* Stores the total size of memory consumed in bytes */
    
    /* The function requires a single argument of type Array with an optional second argument of type bool */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|b", &array, &d) == FAILURE) {
        return;
    }
    
    arSize = get_array_size(array, d);
    
    RETURN_LONG(arSize);
}

PHP_MINFO_FUNCTION(AA)
{
    php_info_print_table_start();
    php_info_print_table_row(2, PHP_AA_EXTNAME, "enabled");
    php_info_print_table_row(2, "version", PHP_AA_EXTVER);
    if (sapi_module.phpinfo_as_text) {
        php_info_print_table_row(2, "By", "Sherif Ramadan\nhttp://sheriframadan.com");
    } else {
    php_printf("<tr>"
               "<td class=\"e\">By</td>"
               "<td class=\"v\">"
               "<a href=\"http://sheriframadan.com\">Sherif Ramadan</a>"
               "</td>"
               "</tr>");
    }
    php_info_print_table_end();
}

static zend_function_entry array_analysis_functions[] = {
	PHP_FE(array_size,      	NULL)
	PHP_FE_END
};

zend_module_entry AA_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_AA_EXTNAME,
	array_analysis_functions, /* Functions */
	NULL, /* MINIT */
	NULL, /* MSHUTDOWN */
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	PHP_MINFO(AA), /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_AA_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_AA
ZEND_GET_MODULE(AA)
#endif
