package com.ir.example;

import dagger.hilt.InstallIn;
import dagger.hilt.codegen.OriginatingElement;
import dagger.hilt.components.SingletonComponent;
import dagger.hilt.internal.GeneratedEntryPoint;

@OriginatingElement(
    topLevelClass = IRApplication.class
)
@GeneratedEntryPoint
@InstallIn(SingletonComponent.class)
public interface IRApplication_GeneratedInjector {
  void injectIRApplication(IRApplication iRApplication);
}
