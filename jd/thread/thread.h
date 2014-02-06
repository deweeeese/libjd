#pragma once

int GetHardwareThreadCount();

bool IsMainThread();

bool ThreadSystemInit();
void ThreadSystemShutdown();

void ThreadLogCurrentId();
