using System;
using System.Text;

namespace PivotalTracker
{
    /// <summary>
    /// A Comment on a Story.
    /// </summary>
    public class StoryComment
    {
        /// <summary>
        /// Comment's ID.
        /// </summary>
        public int ID { get; set; }

        /// <summary>
        /// Comment's Text.
        /// </summary>
        public string Text { get; set; }

        /// <summary>
        /// The Author of the Comment.
        /// </summary>
        public string Author { get; set; }

        /// <summary>
        /// Date of Comment's Creation.
        /// </summary>
        public DateTime CreationDate { get; set; }

        /// <summary>
        /// Is a File attached?
        /// </summary>
        private bool isFileAttached;

        private StoryCommentAttachment attachment = new StoryCommentAttachment();
        /// <summary>
        /// The Attachment of the Comment.
        /// </summary>
        public StoryCommentAttachment Attachment
        {
            get
            {
                return attachment;
            }
            set
            {
                isFileAttached = (value != null);
                attachment = value;
            }
        }

        /// <summary>
        /// Default contructor of StoryComment.
        /// </summary>
        public StoryComment()
        {
        }

        /// <summary>
        /// StoryComment's copy contructor.
        /// </summary>
        /// <param name="comment"></param>
        public StoryComment(StoryComment comment)
        {
            ID = comment.ID;
            Author = comment.Author;
            isFileAttached = (comment.Attachment != null);
            Attachment = comment.Attachment;
        }

        public void RemoveAttachment()
        {
            Attachment = null;
        }

        public string ToXmlString()
        {
            StringBuilder comment = new StringBuilder();
            comment.Append("<note>");
            comment.Append("<id>" + ID + "</id>");
            if (!String.IsNullOrEmpty(Text))
            {
                comment.Append("<text>" + Text + "</text>");
            }
            comment.Append("<author>" + Author + "</author>");
            comment.Append("noted_at" + CreationDate.ToString() + " UTC" + "</noted_at>");
            if (Attachment != null)
            {
                comment.Append("<attachments>" + Attachment.ToXmlString() + "</attachments");
            }
            comment.Append("</note>");
            return comment.ToString();
        }
    }
}
