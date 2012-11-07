using System;
using System.Text;

namespace PivotalTracker
{
    /// <summary>
    /// The Attachment of a Comment.
    /// </summary>
    public class StoryCommentAttachment
    {
        /// <summary>
        /// ID of Attachment.
        /// </summary>
        public int ID { get; set; }
        /// <summary>
        /// Name of the File.
        /// </summary>
        public string FileName { get; set; }
        /// <summary>
        /// escription of the File(the Comment).
        /// </summary>
        public string Description { get; set; }
        /// <summary>
        /// File's Uploader.
        /// </summary>
        public string Uploader { get; set; }
        /// <summary>
        /// Date of Attachment's Uploading.
        /// </summary>
        public DateTime UploadDate { get; set; }
        /// <summary>
        /// URL of the File.
        /// </summary>
        public string Url { get; set; }

        /// <summary>
        /// StoryCommentAttachment's default constructor.
        /// </summary>
        public StoryCommentAttachment()
        {
        }

        /// <summary>
        /// StoryCommentAttachment's copy-constructor.
        /// </summary>
        /// <param name="attachment"></param>
        public StoryCommentAttachment(StoryCommentAttachment attachment)
        {
            ID = attachment.ID;
            FileName = attachment.FileName;
            Description = attachment.Description;
            Uploader = attachment.Uploader;
            UploadDate = attachment.UploadDate;
            Url = attachment.Url;
        }

        public string ToXmlString()
        {
            StringBuilder attachment = new StringBuilder();
            attachment.Append("<attachment>");
            attachment.Append("<id>" + ID + "</id>");
            attachment.Append("<filename>" + FileName.ToLower() + "</filename>");
            attachment.Append("<description>" + Description + "</description>");
            attachment.Append("<uploaded_by>" + Uploader + "</uploaded_by>");
            attachment.Append("<uploaded_at>" + UploadDate.ToString() + " UTC" + "</uploaded_at>");
            attachment.Append("<url>" + Url + "</url>");
            attachment.Append("</attachment>");
            return attachment.ToString();
        }
    }
}
