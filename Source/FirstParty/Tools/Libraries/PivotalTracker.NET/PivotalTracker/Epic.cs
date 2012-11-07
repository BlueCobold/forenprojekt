using System.Collections.Generic;
using System.Net;
using System.IO;

namespace PivotalTracker
{
    /// <summary>
    /// An Epic is a collection of Stories with a equivalent Label.
    /// </summary>
    public class Epic : Refreshable
    {
        /// <summary>
        /// Epic's Name.
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Epic's ID.
        /// </summary>
        public int ID { get; set; }

        /// <summary>
        /// The Web-URL of the Epic.
        /// </summary>
        public string Url { get; set; }

        /// <summary>
        /// Storys with this Label will become a part of the Epic.
        /// </summary>
        public string LinkedLabel { get; set; }

        /// <summary>
        /// Epic's Description.
        /// </summary>
        public string Description { get; set; }

        /// <summary>
        /// All Comments(+Attachments) for this Story.
        /// </summary>
        public List<StoryCommentAttachment> Comments { get; set; }

        public Epic()
        {
            OnRefresh += OnEpicUpdate;
        }

        private void OnEpicUpdate(UserAuthentication authentication)
        {

        }
    }
}
