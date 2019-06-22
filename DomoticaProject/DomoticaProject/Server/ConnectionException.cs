using System;
using System.Collections.Generic;
using System.Text;

namespace DomoticaProject.Server {
    public class ConnectionException : Exception {
        public ConnectionException() : base() { }
        public ConnectionException(string message) : base(message) { }
    }
}
