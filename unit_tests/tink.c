#include <stdio.h>
#include "CUnit/Basic.h"
#include "tink_api.h"

void TestInitTink()
{
    CU_ASSERT(0==InitTink(NULL,NULL));
}

void TestInitTink2()
{
    CU_ASSERT(1==InitTink(NULL,NULL));
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   //pSuite = CU_add_suite("Suite_Tink", TestInitTink, TestInitTink2);
   pSuite = CU_add_suite("Suite_Tink", NULL, NULL);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test for pass", Encrypt(====))) || //TODO: create tests
       (NULL == CU_add_test(pSuite, "test for fail", Decrypt(====))))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

