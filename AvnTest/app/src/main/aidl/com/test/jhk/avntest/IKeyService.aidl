// IKeyService.aidl
package com.test.jhk.avntest;

// Declare any non-default types here with import statements
import com.test.jhk.avntest.IKeyServiceCallback;

interface IKeyService {
    void registerCallback(IKeyServiceCallback cb);
}
