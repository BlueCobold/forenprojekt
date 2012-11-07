using System;
using System.Collections.Generic;
using System.Net;
using System.IO;
using System.Text;

namespace PivotalTracker
{
    /// <summary>
    /// A Story represents a task and the work on it.
    /// </summary>
    public class Story
    {
        /// <summary>
        /// Story's Name.
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Date of the Story's Creation.
        /// </summary>
        public DateTime CreationDate { get; set; }

        /// <summary>
        /// Date of the Story's last Update.
        /// </summary>
        public DateTime LastUpdateDate { get; set; }

        /// <summary>
        /// Story's ID.
        /// </summary>
        public int ID { get; set; }

        /// <summary>
        /// The Web-URL of the Story.
        /// </summary>
        public string Url { get; set; }

        /// <summary>
        /// Story's Type.
        /// </summary>
        public StoryType Type { get; set; }

        /// <summary>
        /// How many points the Story is worth.
        /// </summary>
        public int Points { get; set; }

        /// <summary>
        /// The current State of the Story.
        /// </summary>
        public StoryState State { get; set; }

        /// <summary>
        /// The Requestor of the Story.
        /// </summary>
        public string Requestor { get; set; }

        /// <summary>
        ///The Owner of the Story.
        /// </summary>
        public string Owner { get; set; }

        /// <summary>
        /// Story's Description.
        /// </summary>
        public string Description { get; set; }

        /// <summary>
        /// Story's Labels.
        /// </summary>
        public List<string> Labels { get; set; }

        /// <summary>
        /// All Tasks of this Story.
        /// </summary>
        public List<Task> Tasks { get; set; }

        /// <summary>
        /// All Comments(+Attachments) for this Story.
        /// </summary>
        public List<StoryComment> Comments { get; set; }

        public Story()
        {
            Name = "";
            Url = "";
            Type = StoryType.Feature;
            State = StoryState.Unstarted;
            Requestor = "";
            Owner = "";
            Description = "";
            Tasks = new List<Task>();
            Labels = new List<string>();
            Comments = new List<StoryComment>();
        }

        public Story(Story story)
        {
            Name = story.Name;
            CreationDate = story.CreationDate;
            LastUpdateDate = story.LastUpdateDate;
            ID = story.ID;
            Url = story.Url;
            Type = story.Type;
            Points = story.Points;
            State = story.State;
            Requestor = story.Requestor;
            Owner = story.Owner;
            Description = story.Description;
            Labels = story.Labels;
            Tasks = story.Tasks;
            Comments = story.Comments;
        }
        /// <summary>
        /// Creates a new Task for this Story.
        /// </summary>
        /// <param name="description"></param>
        public void CreateNewTask(string description)
        {
            Task task = new Task
            {
                Description = description,
                CreationDate = DateTime.Now
            };
            Tasks.Add(task);
        }

        public string ToXmlString(UserAuthentication authentication)
        {
            string url = "http://www.pivotaltracker.com/services/v3/projects/" + ID + "/stories?token=" + authentication.Token;
            StringBuilder storyAsXml = new StringBuilder("<story>");
            storyAsXml.Append("<id>" + ID.ToString() + "</id>");
            storyAsXml.Append("<story_type>" + Type.ToString().ToLower() + "</story_type>");
            storyAsXml.Append("<url>" + Url.ToString().ToLower() + "</url>");
            storyAsXml.Append("<estimate>" + Points.ToString() + "</estimate>");
            storyAsXml.Append("<current_state>" + State.ToString().ToLower() + "</current_state>");
            if (!String.IsNullOrEmpty(Description))
            {
                storyAsXml.Append("<description>" + Description + "</description>");
            }
            storyAsXml.Append("<name>" + Name + "</name>");
            storyAsXml.Append("<requested_by>" + Requestor + "</requested_by>");
            storyAsXml.Append("<owned_by>" + Owner + "</owned_by>");
            storyAsXml.Append("<created_at>" + CreationDate.ToString() + " UTC" + "</created_at>");
            storyAsXml.Append("<updated_at>" + LastUpdateDate.ToString() + " UTC" + "</updated_at>");
            string labels = "";
            foreach (string label in Labels)
            {
                if (labels != "")
                    labels += ",";
                labels += label;
            }
            if (!String.IsNullOrEmpty(labels))
            {
                storyAsXml.Append("<labels>" + labels + "</labels>");
            }
            StringBuilder tasks = new StringBuilder();
            foreach (Task task in Tasks)
            {
                tasks.Append(task.ToXmlString());
            }
            if (!String.IsNullOrEmpty(tasks.ToString()))
            {
                storyAsXml.Append("<tasks>" + tasks.ToString() + "</tasks>");
            }
            StringBuilder comments = new StringBuilder();
            foreach (StoryComment comment in Comments)
            {
                comments.Append(comment.ToXmlString());
            }
            if (!String.IsNullOrEmpty(comments.ToString()))
            {
                storyAsXml.Append("<notes>" + comments.ToString() + "</notes>");
            }
            storyAsXml.Append("</story>");
            return storyAsXml.ToString();
        }
    }
}