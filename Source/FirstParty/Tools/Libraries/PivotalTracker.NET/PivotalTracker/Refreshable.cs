using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PivotalTracker
{
    public abstract class Refreshable
    {
        public delegate void RefreshHandler(UserAuthentication authentication);
        public event RefreshHandler OnRefresh;
        public RefreshMode mode { get; set; }

        public void Refresh(UserAuthentication authentication)
        {
            OnRefresh(authentication);
        }
    }
}
