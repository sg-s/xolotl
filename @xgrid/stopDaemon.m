function stopDaemon(self)

stop(self.daemon_handle);
delete(self.daemon_handle);

% trigger a rebuild in the cpplab cache
cpplab.rebuildCache()